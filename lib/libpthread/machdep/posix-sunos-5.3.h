/* ==== posix.h ============================================================
 * Copyright (c) 1993 by Chris Provenzano, proven@athena.mit.edu	
 *
 * $Id: posix-sunos-5.3.h,v 1.2 1995/01/06 07:28:13 raeburn Exp $
 *
 * Description : Do the right thing for a sunos 4.1.3 system.
 *
 *  1.00 93/07/20 proven
 *      -Started coding this file.
 */

#ifndef _PTHREAD_POSIX_H_
#define _PTHREAD_POSIX_H_

/* Make sure we have size_t defined */
#include <pthread/types.h>

#ifndef __WAIT_STATUS
#define __WAIT_STATUS   int *
#endif

#endif
