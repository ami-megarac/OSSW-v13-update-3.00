--- linux/drivers/i2c/busses/i2c-new-aspeed.c	2021-04-14 04:49:28.183469128 +0530
+++ linux_new/drivers/i2c/busses/i2c-new-aspeed.c	2021-04-14 04:51:47.921235434 +0530
@@ -549,6 +549,7 @@
 int aspeed_new_i2c_slave_irq(struct aspeed_new_i2c_bus *i2c_bus)
 {
 	u32 cmd = 0;
+	u32 ier = 0 ;
 	int ret = 0;
 	u8 value;
 	int i = 0;
@@ -812,9 +813,14 @@
 				dev_dbg(i2c_bus->dev, "S : Sr|D \n");
 				byte_data = AST_I2CC_GET_RX_BUFF(aspeed_i2c_read(i2c_bus, AST_I2CC_STS_AND_BUFF));
 				dev_dbg(i2c_bus->dev, "addr : [%02x]", byte_data);
+				ier = aspeed_i2c_read(i2c_bus, AST_I2CS_IER);
+				ier  &= ~(AST_I2CS_RX_DONE | AST_I2CS_SLAVE_MATCH); 
+				sts &= ~ ( AST_I2CS_SLAVE_MATCH | AST_I2CS_RX_DONE | AST_I2CS_Wait_TX_DMA );
+				aspeed_i2c_write(i2c_bus, ier , AST_I2CS_IER);
 				i2c_slave_event(i2c_bus->slave, I2C_SLAVE_READ_REQUESTED, &byte_data);
-				dev_dbg(i2c_bus->dev, "tx: [%02x]\n", byte_data);
-				aspeed_i2c_write(i2c_bus, byte_data, AST_I2CC_STS_AND_BUFF);
+				// dev_dbg(i2c_bus->dev, "tx: [%02x]\n", byte_data);
+				// aspeed_i2c_write(i2c_bus, byte_data, AST_I2CC_STS_AND_BUFF);
+				dev_dbg(i2c_bus->dev, "tx: req\n");
 				break;
 			case AST_I2CS_TX_ACK | AST_I2CS_Wait_TX_DMA:
 				cmd |= AST_I2CS_TX_CMD;
@@ -824,8 +830,21 @@
 				aspeed_i2c_write(i2c_bus, byte_data, AST_I2CC_STS_AND_BUFF);
 				break;
 			case AST_I2CS_STOP:
+			case AST_I2CS_RX_DONE | AST_I2CS_RX_DONE_NAK | AST_I2CS_STOP:
 			case AST_I2CS_STOP | AST_I2CS_TX_NAK:
 				dev_dbg(i2c_bus->dev, "S : P \n");
+				value = sts ;
+				dev_dbg(i2c_bus->dev, "value=%x \n",value);
+				if(sts & AST_I2CS_RX_DONE)
+				{
+					dev_dbg(i2c_bus->dev, "S : D \n");
+					byte_data = AST_I2CC_GET_RX_BUFF(aspeed_i2c_read(i2c_bus, AST_I2CC_STS_AND_BUFF));
+					i2c_slave_event(i2c_bus->slave, I2C_SLAVE_WRITE_RECEIVED, &byte_data);
+					dev_dbg(i2c_bus->dev, "rx [%x]", byte_data);
+				}
+				if(sts & AST_I2CS_RX_DONE_NAK)
+					dev_dbg(i2c_bus->dev, "rx: NACK\n");
+
 				i2c_slave_event(i2c_bus->slave, I2C_SLAVE_STOP, &value);
 				break;
 			default:
