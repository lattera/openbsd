/* ==== posix.h ============================================================
 * Copyright (c) 1993 by Chris Provenzano, proven@athena.mit.edu	
 *
 * $Id: posix-ultrix-4.2.h,v 1.51 1995/01/06 07:28:38 raeburn Exp $
 *
 * Description : Convert an Ultrix-4.2 system to a more or less POSIX system.
 *
 *  1.00 93/07/20 proven
 *      -Started coding this file.
 */

#ifndef _PTHREAD_POSIX_H_
#define _PTHREAD_POSIX_H_

#include <sys/cdefs.h>

/* Make sure we have size_t defined */
#include <pthread/types.h>

#ifndef __WAIT_STATUS
#define __WAIT_STATUS 	int *
#endif

#endif
