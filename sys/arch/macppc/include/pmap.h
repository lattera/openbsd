/*	$OpenBSD: src/sys/arch/macppc/include/pmap.h,v 1.3.4.2 2002/03/28 10:36:01 niklas Exp $	*/

#include <powerpc/pmap.h>

#ifndef	_LOCORE
paddr_t vtophys(vaddr_t);
#endif	/* _LOCORE */
