/*------------------------------------------------------------------
 * safe_lib_errno.h -- Safe C Lib Error codes
 * @configure_input@
 *
 * October 2008, Bo Berry
 * Modified 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 * September 2017, Reini Urban
 *
 * Copyright (c) 2008-2013 by Cisco Systems, Inc
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#ifndef __SAFE_LIB_ERRNO_H__
#define __SAFE_LIB_ERRNO_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __KERNEL__
#include <linux/errno.h>
#else
#ifndef SPX_BMC
@INSERT_ERRNO_H@
#else
#include <errno.h>
#endif
#endif /* __KERNEL__ */

/*
 * Safe Lib specific errno codes.  These can be added to the errno.h file
 * if desired. strerror/perror does not know about these errors though,
 * and it's messages are not extendable, only strerror_s() does.
 */

#ifndef ESNULLP
#define ESNULLP         ( 400 )       /* null ptr                    */
#endif

#ifndef ESZEROL
#define ESZEROL         ( 401 )       /* length is zero              */
#endif

#ifndef ESLEMIN
#define ESLEMIN         ( 402 )       /* length is below min         */
#endif

#ifndef ESLEMAX
#define ESLEMAX         ( 403 )       /* length exceeds RSIZE_MAX    */
#endif

#ifndef ESOVRLP
#define ESOVRLP         ( 404 )       /* overlap undefined           */
#endif

#ifndef ESEMPTY
#define ESEMPTY         ( 405 )       /* empty string                */
#endif

#ifndef ESNOSPC
#define ESNOSPC         ( 406 )       /* not enough space for s2     */
#endif

#ifndef ESUNTERM
#define ESUNTERM        ( 407 )       /* unterminated string         */
#endif

#ifndef ESNODIFF
#define ESNODIFF        ( 408 )       /* no difference               */
#endif

#ifndef ESNOTFND
#define ESNOTFND        ( 409 )       /* not found                   */
#endif

#ifndef ESLEWRNG
#define ESLEWRNG        ( 410 )       /* wrong size                */
#endif

#ifndef ESLAST
#define ESLAST ESLEWRNG
#endif

/* EOK may or may not be defined in errno.h */
#ifndef EOK
#define EOK             ( 0 )
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_LIB_ERRNO_H__ */
