/*	$OpenBSD: src/sys/arch/macppc/include/pmap.h,v 1.3.6.1 2002/06/11 03:36:34 art Exp $	*/

#include <powerpc/pmap.h>

#ifndef	_LOCORE
paddr_t vtophys(vaddr_t);
#endif	/* _LOCORE */
