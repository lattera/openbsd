/*	$OpenBSD: src/sys/arch/mvmeppc/include/Attic/pmap.h,v 1.1.2.1 2001/10/31 03:01:21 nate Exp $	*/

#include <powerpc/pmap.h>

#ifndef	_LOCORE
paddr_t vtophys __P((vaddr_t));
#endif	/* _LOCORE */
