/*	$OpenBSD: src/sys/arch/hp300/include/pte.h,v 1.5 2002/01/10 21:10:47 miod Exp $	*/

#ifndef	_MACHINE_PTE_H_
#define	_MACHINE_PTE_H_

#include <m68k/pte_motorola.h>

#define	MACHINE_STSIZE		M68K_STSIZE
#define	MACHINE_MAX_PTSIZE	M68K_MAX_PTSIZE
#define	MACHINE_MAX_KPTSIZE	M68K_MAX_KPTSIZE
#define	MACHINE_PTBASE		M68K_PTBASE
#define	MACHINE_PTMAXSIZE	M68K_PTMAXSIZE

#endif	/* _MACHINE_PTE_H_ */
