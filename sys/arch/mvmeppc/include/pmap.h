/*	$OpenBSD: src/sys/arch/mvmeppc/include/Attic/pmap.h,v 1.5.4.1 2002/06/11 03:37:22 art Exp $	*/

#include <powerpc/pmap.h>

#ifndef	_LOCORE
paddr_t vtophys(vaddr_t);
#endif	/* _LOCORE */
