/*	$OpenBSD: src/sys/arch/mvmeppc/include/Attic/pmap.h,v 1.1.2.2 2002/03/28 10:36:03 niklas Exp $	*/

#include <powerpc/pmap.h>

#ifndef	_LOCORE
paddr_t vtophys(vaddr_t);
#endif	/* _LOCORE */
