/*	$OpenBSD: src/sys/arch/macppc/include/intr.h,v 1.2.4.1 2001/10/31 03:01:16 nate Exp $	*/

#include <powerpc/intr.h>
#ifndef _LOCORE
void softtty(void);
#endif
