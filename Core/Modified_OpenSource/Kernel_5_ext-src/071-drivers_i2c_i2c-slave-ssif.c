--- linux/drivers/i2c/i2c-slave-ssif.c	2021-04-14 04:58:34.106899361 +0530
+++ linux_new/drivers/i2c/i2c-slave-ssif.c	2021-04-14 04:51:47.889235941 +0530
@@ -0,0 +1,447 @@
+#include <linux/i2c.h>
+#include <linux/kernel.h>
+#include <linux/module.h>
+#include <linux/of.h>
+#include <linux/slab.h>
+#include <linux/spinlock.h>
+#include <linux/sysfs.h>
+#include <linux/io.h>
+#include <linux/fs.h>
+#include <linux/miscdevice.h>
+
+#define SSIF_SLAVE_DEVICE_NAME		"ssif-slave-mqueue"
+#define AST_I2CC_FUN_CTRL			0x00
+#define MQ_MSGBUF_SIZE		128
+#define MQ_QUEUE_SIZE		32
+#define MQ_QUEUE_NEXT(x)	(((x) + 1) & (MQ_QUEUE_SIZE - 1))
+#define AST_I2CS_TX_NAK				BIT(1)
+#define AST_I2CS_RX_DONE_NAK		BIT(3)
+#define AST_I2CS_STOP				BIT(4)
+#define AST_I2CS_TX_CMD					BIT(2)
+#define AST_I2CS_RX_DONE			BIT(2)
+#define AST_I2CS_SLAVE_MATCH		BIT(7)
+#define AST_I2CS_CMD_STS		0x28	/* 0x28 : I2CS Slave CMD/Status Register   */
+#define AST_I2CS_ISR			0x24	/* 0x24 : I2CS Slave Interrupt Status Register   */
+#define AST_I2CS_IER			0x20	/* 0x20 : I2CS Slave Interrupt Control Register   */
+#define AST_I2CC_STS_AND_BUFF		0x08	/* 0x08 : I2CC Master/Slave Transmit/Receive Byte Buffer Register */
+#define AST_I2CS_Wait_TX_DMA		BIT(25)
+#define AST_I2CS_RX_DONE			BIT(2)
+#define AST_I2CC_MASTER_EN				BIT(0)
+#define AST_SSIF_NACK_DISABLE    0x0761
+#define IPMI_SSIF_SINGLE_PART_WRITE_SSIF_CMD			0x02  //W
+#define IPMI_SSIF_MULTI_PART_WRITE_START_SSIF_CMD	0x06  //W
+#define IPMI_SSIF_MULTI_PART_WRITE_MIDDLE_SSIF_CMD	0x07  //W
+#define IPMI_SSIF_MULTI_PART_WRITE_END_SSIF_CMD		0x08  //R
+#define IPMI_SSIF_READ_START_SMBUS_CMD				0x03  //R
+#define IPMI_SSIF_READ_MIDDLE_SMBUS_CMD				0x09  //R
+#define IPMI_SSIF_READ_RETRY_SSIF_CMD					0x0A  //W
+#define AST_I2CS_RX_CMD_LAST			BIT(4)
+
+
+struct mq_msg {
+	int	len;
+	u8	*buf;
+};
+enum xfer_mode {
+	BYTE_MODE = 0,
+	BUFF_MODE,
+	DMA_MODE,
+
+};
+
+struct aspeed_new_i2c_bus {
+	struct device		*dev;
+	void __iomem		*reg_base;
+	struct regmap		*global_reg;
+	int 				irq;
+	enum xfer_mode 		mode; //0: dma, 1: pool, 2:byte
+	int					clk_div_mode;	//0: old mode, 1: new mode
+	struct clk 			*clk;
+	u32					apb_clk;
+	u32					bus_frequency;
+	u32					state;			//I2C xfer mode state matchine
+	u32					bus_recover;
+	struct i2c_adapter	adap;
+	/* Multi-master */
+	bool				multi_master;
+	/* master structure */	
+	int					cmd_err;
+	struct completion	cmd_complete;
+	struct i2c_msg 		*msgs;	//cur xfer msgs
+	size_t				buf_index;	//buffer mode idx 	
+	int					msgs_index;	//cur xfer msgs index
+	int					msgs_count;	//total msgs
+	dma_addr_t 			master_dma_addr;
+	int					master_xfer_cnt;	//total xfer count
+	/* Buffer mode */
+	void __iomem		*buf_base;
+	size_t				buf_size;
+	/* Slave structure */
+	int 				slave_xfer_len;
+	int 				slave_xfer_cnt;
+#ifdef CONFIG_I2C_SLAVE
+	unsigned char		*slave_dma_buf;
+	dma_addr_t			slave_dma_addr;
+	struct i2c_client 	*slave;
+#endif
+};
+
+static inline void
+aspeed_i2c_write(struct aspeed_new_i2c_bus *i2c_bus, u32 val, u32 reg)
+{
+	//	dev_dbg(i2c_bus->dev, "aspeed_i2c_write : val: %x , reg : %x \n",val,reg);
+	writel(val, i2c_bus->reg_base + reg);
+}
+
+static inline u32
+aspeed_i2c_read(struct aspeed_new_i2c_bus *i2c_bus, u32 reg)
+{
+#if 0
+	u32 val = readl(i2c_bus->reg_base + reg);
+	printk("R : reg %x , val: %x \n", reg, val);
+	return val;
+#else
+	return readl(i2c_bus->reg_base + reg);
+#endif
+}
+
+static DECLARE_WAIT_QUEUE_HEAD(as_slave_data_wait);
+struct mq_queue {
+	struct bin_attribute	bin;
+	struct miscdevice miscdev;
+	struct kernfs_node	*kn;
+
+	spinlock_t		lock; /* spinlock for queue index handling */
+	int			in;
+	int			out;
+	int 		wr_len;  
+	int 		nackstatus;
+
+	struct mq_msg		*curr;
+	int			truncated; /* drop current if truncated */
+	struct mq_msg		queue[MQ_QUEUE_SIZE];
+	struct mq_msg		wr_queue;
+};
+
+static int ssif_slave_mqueue_callback(struct i2c_client *client,
+		enum i2c_slave_event event, u8 *val)
+{
+	struct mq_queue *mq = i2c_get_clientdata(client);
+	struct mq_msg *msg = mq->curr;
+	struct mq_msg *wr_msg = & mq->wr_queue;
+	int ret = 0 , len = 0 ,cmd = 0;
+	struct aspeed_new_i2c_bus *i2c_bus = i2c_get_adapdata(client->adapter);
+
+	switch (event) {
+	case I2C_SLAVE_WRITE_REQUESTED:
+		mq->truncated = 0;
+
+		msg->len = 1;
+		msg->buf[0] = client->addr << 1;
+		spin_lock(&mq->lock);
+		if(mq->nackstatus)
+		{
+			cmd = aspeed_i2c_read(i2c_bus, AST_I2CS_CMD_STS);
+			cmd |= AST_I2CS_RX_CMD_LAST ;
+			aspeed_i2c_write(i2c_bus, cmd, AST_I2CS_CMD_STS);
+		}
+		spin_unlock(&mq->lock);
+
+		break;
+
+	case I2C_SLAVE_WRITE_RECEIVED:
+		if (msg->len < MQ_MSGBUF_SIZE) {
+			if(msg->len == 1 && (*val == IPMI_SSIF_SINGLE_PART_WRITE_SSIF_CMD || *val == IPMI_SSIF_MULTI_PART_WRITE_END_SSIF_CMD))
+			{
+				spin_lock(&mq->lock);
+				mq->nackstatus = 0;
+				spin_unlock(&mq->lock);
+			}
+
+			msg->buf[msg->len++] = *val;
+		} else {
+			dev_err(&client->dev, "message is truncated!\n");
+			mq->truncated = 1;
+			ret = -EINVAL;
+		}
+		break;
+	case I2C_SLAVE_READ_REQUESTED:  
+		spin_lock(&mq->lock);
+		mq->in = MQ_QUEUE_NEXT(mq->in);
+		mq->curr = &mq->queue[mq->in];
+		mq->curr->len = 0;
+
+		/* Flush the oldest message */
+		if (mq->out == mq->in)
+			mq->out = MQ_QUEUE_NEXT(mq->out);
+		spin_unlock(&mq->lock);
+
+		kernfs_notify(mq->kn);
+
+		break;
+	case I2C_SLAVE_READ_PROCESSED:
+		mq->wr_len++;
+		if(mq->wr_len < wr_msg->len )
+			*val = wr_msg->buf[mq->wr_len] ;
+		else
+		{
+//			/printk("mq->wr_len =%d,wr_msg->len=%d\n",mq->wr_len,wr_msg->len);
+			*val = 0xFF ;
+		}
+
+		break;
+	case I2C_SLAVE_STOP:
+		if(*val & AST_I2CS_TX_NAK )
+		{
+			spin_lock(&mq->lock);
+			mq->wr_len = 0 ;
+			spin_unlock(&mq->lock);
+		}
+		else if ( !(*val &  AST_I2CS_RX_DONE_NAK ) ) {
+			if (unlikely(mq->truncated || msg->len < 2))
+				break;
+			if(msg->buf[1] == IPMI_SSIF_SINGLE_PART_WRITE_SSIF_CMD || msg->buf[1] == IPMI_SSIF_MULTI_PART_WRITE_END_SSIF_CMD)
+			{
+				spin_lock(&mq->lock);
+				mq->nackstatus = 1;
+				spin_unlock(&mq->lock);
+			}
+
+			spin_lock(&mq->lock);
+			mq->in = MQ_QUEUE_NEXT(mq->in);
+			mq->curr = &mq->queue[mq->in];
+			mq->curr->len = 0;
+
+			/* Flush the oldest message */
+			if (mq->out == mq->in)
+				mq->out = MQ_QUEUE_NEXT(mq->out);
+			spin_unlock(&mq->lock);
+
+			kernfs_notify(mq->kn);
+		}
+		break;
+
+	default:
+		*val = 0xFF;
+		dev_err(&client->dev, "unknown irq event\n");
+		break;
+	}
+
+	return ret;
+}
+
+static ssize_t i2c_slave_mqueue_bin_write(struct file *filp,
+		struct kobject *kobj,
+		struct bin_attribute *attr,
+		char *buf, loff_t pos, size_t count)
+{
+	struct mq_queue *mq;
+	struct mq_msg *msg;
+	unsigned long flags;
+	int ier = 0 , cmd = 0 ,sts = 0;
+	struct i2c_client	*slave;
+	struct aspeed_new_i2c_bus *i2c_bus;
+
+	mq = dev_get_drvdata(container_of(kobj, struct device, kobj));
+	slave = kobj_to_i2c_client(kobj);
+
+	i2c_bus = i2c_get_adapdata(slave->adapter);
+	spin_lock_irqsave(&mq->lock, flags);
+	msg = &mq->wr_queue;
+	memcpy(msg->buf,buf,count);
+	msg->len = count ;
+
+	spin_unlock_irqrestore(&mq->lock, flags);
+	dev_dbg(i2c_bus->dev, "len tx: [%02x]\n",msg->buf[0] );
+	aspeed_i2c_write(i2c_bus,msg->buf[0], AST_I2CC_STS_AND_BUFF);
+	sts =  aspeed_i2c_read(i2c_bus, AST_I2CS_ISR);
+	sts |= ( AST_I2CS_SLAVE_MATCH | AST_I2CS_RX_DONE | AST_I2CS_Wait_TX_DMA ) ;
+	aspeed_i2c_write(i2c_bus, sts, AST_I2CS_ISR);
+	ier = aspeed_i2c_read(i2c_bus, AST_I2CS_IER);
+	ier  |= (AST_I2CS_RX_DONE | AST_I2CS_SLAVE_MATCH); 
+	aspeed_i2c_write(i2c_bus, ier , AST_I2CS_IER);
+	cmd = aspeed_i2c_read(i2c_bus, AST_I2CS_CMD_STS);
+	cmd |= AST_I2CS_TX_CMD ;
+	aspeed_i2c_write(i2c_bus, cmd, AST_I2CS_CMD_STS);
+
+
+
+	return count;
+}
+
+static ssize_t i2c_slave_mqueue_bin_read(struct file *filp,
+		struct kobject *kobj,
+		struct bin_attribute *attr,
+		char *buf, loff_t pos, size_t count)
+{
+	struct mq_queue *mq;
+	struct mq_msg *msg;
+	unsigned long flags;
+	bool more = false;
+	ssize_t ret = 0;
+	mq = dev_get_drvdata(container_of(kobj, struct device, kobj));
+
+	spin_lock_irqsave(&mq->lock, flags);
+	if (mq->out != mq->in) {
+		msg = &mq->queue[mq->out];
+
+		if (msg->len <= count) {
+			ret = msg->len;
+			memcpy(buf, msg->buf, ret);
+		} else {
+			ret = -EOVERFLOW; /* Drop this HUGE one. */
+		}
+
+		mq->out = MQ_QUEUE_NEXT(mq->out);
+		if (mq->out != mq->in)
+			more = true;
+	}
+	spin_unlock_irqrestore(&mq->lock, flags);
+
+	if (more)
+		kernfs_notify(mq->kn);
+
+
+
+	return ret;
+}
+
+static long ssif_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
+{
+	int ret = 0;
+	struct mq_queue *mq;
+	unsigned long flags;
+	mq = container_of(file->private_data, struct mq_queue, miscdev);
+	switch (cmd) {
+	case AST_SSIF_NACK_DISABLE:
+		spin_lock_irqsave(&mq->lock, flags);
+		mq->nackstatus=0;
+		spin_unlock_irqrestore(&mq->lock, flags);
+		break;
+	default: /* unknown ioctl number */
+		break;
+	}
+	return ret;
+}
+
+static const struct file_operations ssif_fops = {
+		.owner	= THIS_MODULE,
+		.unlocked_ioctl	= ssif_ioctl,
+};
+
+static int ssif_slave_mqueue_probe(struct i2c_client *client,
+		const struct i2c_device_id *id)
+{
+	struct device *dev = &client->dev;
+	struct mq_queue *mq;
+	int ret, i;
+	void *buf, *wr_buf;
+	struct aspeed_new_i2c_bus *i2c_bus = i2c_get_adapdata(client->adapter);
+
+	dev_dbg(i2c_bus->dev,"ssif_slave_mqueue_probe with id->name:%s, with data:0x%08lx\n",id->name,id->driver_data);
+	mq = devm_kzalloc(dev, sizeof(*mq), GFP_KERNEL);
+	if (!mq)
+		return -ENOMEM;
+
+	BUILD_BUG_ON(!is_power_of_2(MQ_QUEUE_SIZE));
+
+	buf = devm_kmalloc_array(dev, MQ_QUEUE_SIZE, MQ_MSGBUF_SIZE,
+			GFP_KERNEL);
+	if (!buf)
+		return -ENOMEM;
+
+	wr_buf = devm_kmalloc_array(dev, 1, MQ_MSGBUF_SIZE,
+			GFP_KERNEL);
+	if (!wr_buf)
+		return -ENOMEM;
+
+	for (i = 0; i < MQ_QUEUE_SIZE; i++)
+	{
+		mq->queue[i].buf = buf + i * MQ_MSGBUF_SIZE;
+	}
+	mq->wr_queue.buf = wr_buf ;
+
+	i2c_set_clientdata(client, mq);
+
+	spin_lock_init(&mq->lock);
+	mq->curr = &mq->queue[0];
+
+	sysfs_bin_attr_init(&mq->bin);
+	mq->bin.attr.name = SSIF_SLAVE_DEVICE_NAME;
+	mq->bin.attr.mode = S_IRUSR | S_IWUSR;
+	mq->bin.read = i2c_slave_mqueue_bin_read;
+	mq->bin.write = i2c_slave_mqueue_bin_write;
+
+	mq->bin.size = MQ_MSGBUF_SIZE * MQ_QUEUE_SIZE;
+
+	ret = sysfs_create_bin_file(&dev->kobj, &mq->bin);
+	if (ret)
+		return ret;
+
+	mq->kn = kernfs_find_and_get(dev->kobj.sd, mq->bin.attr.name);
+	if (!mq->kn) {
+		sysfs_remove_bin_file(&dev->kobj, &mq->bin);
+		return -EFAULT;
+	}
+
+	ret = i2c_slave_register(client, ssif_slave_mqueue_callback);
+	if (ret) {
+		kernfs_put(mq->kn);
+		sysfs_remove_bin_file(&dev->kobj, &mq->bin);
+		return ret;
+	}
+	aspeed_i2c_write(i2c_bus, ~ AST_I2CC_MASTER_EN & aspeed_i2c_read(i2c_bus, AST_I2CC_FUN_CTRL), AST_I2CC_FUN_CTRL);
+
+	mq->miscdev.minor = MISC_DYNAMIC_MINOR;
+
+	mq->miscdev.name = devm_kasprintf(&client->dev, GFP_KERNEL,
+			"%s%d", "ssif-",
+			client->adapter->nr);
+	mq->miscdev.fops = &ssif_fops;
+	mq->miscdev.parent = &client->dev;
+	ret = misc_register(&mq->miscdev);
+	if (ret)
+		return ret;
+
+	return 0;
+}
+
+static int ssif_slave_mqueue_remove(struct i2c_client *client)
+{
+	struct mq_queue *mq = i2c_get_clientdata(client);
+
+	i2c_slave_unregister(client);
+	misc_deregister(&mq->miscdev);
+
+	kernfs_put(mq->kn);
+	sysfs_remove_bin_file(&client->dev.kobj, &mq->bin);
+
+	return 0;
+}
+
+static const struct i2c_device_id ssif_slave_mqueue_id[] = {
+		{ SSIF_SLAVE_DEVICE_NAME, 0 },
+		{ }
+};
+MODULE_DEVICE_TABLE(i2c, ssif_slave_mqueue_id);
+
+static const struct of_device_id ssif_slave_of_match[] = {
+		{ .compatible = "slave-ssif", },
+		{ }
+};
+MODULE_DEVICE_TABLE(of, ssif_slave_of_match);
+
+static struct i2c_driver ssif_slave_mqueue_driver = {
+		.driver = {
+				.name	= SSIF_SLAVE_DEVICE_NAME,
+				.of_match_table = ssif_slave_of_match,
+		},
+		.probe		= ssif_slave_mqueue_probe,
+		.remove		= ssif_slave_mqueue_remove,
+		.id_table	= ssif_slave_mqueue_id,
+};
+module_i2c_driver(ssif_slave_mqueue_driver);
+
+MODULE_LICENSE("GPL v2");
+MODULE_AUTHOR("M Hemanth Kumar <hemanthkumarm at amiindia.co.in>");
+MODULE_DESCRIPTION("I2C SSIF slave mode for receiving and queuing messages");
