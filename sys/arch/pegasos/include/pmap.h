/*	$OpenBSD: src/sys/arch/pegasos/include/Attic/pmap.h,v 1.1 2003/10/31 03:54:33 drahn Exp $	*/

#include <powerpc/pmap.h>

#ifndef	_LOCORE
paddr_t vtophys(vaddr_t);
#endif	/* _LOCORE */
