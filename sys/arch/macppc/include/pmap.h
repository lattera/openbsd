/*	$OpenBSD: src/sys/arch/macppc/include/pmap.h,v 1.3.4.1 2001/10/31 03:01:16 nate Exp $	*/

#include <powerpc/pmap.h>

#ifndef	_LOCORE
paddr_t vtophys __P((vaddr_t));
#endif	/* _LOCORE */
