/*	$OpenBSD: src/sys/arch/mvme68k/include/pmap.h,v 1.5.10.2 2001/12/05 00:39:11 niklas Exp $	*/

#ifndef	_MACHINE_PMAP_H_
#define	_MACHINE_PMAP_H_

#include <m68k/pmap_motorola.h>

#ifdef	_KERNEL
void pmap_init_md __P((void));
#define	PMAP_INIT_MD()	pmap_init_md()
#endif

#endif	/* _MACHINE_PMAP_H_ */
