/*	$OpenBSD: src/sys/arch/pegasos/include/Attic/pmap.h,v 1.1.2.1 2004/02/19 10:49:55 niklas Exp $	*/

#include <powerpc/pmap.h>

#ifndef	_LOCORE
paddr_t vtophys(vaddr_t);
#endif	/* _LOCORE */
