/*	$OpenBSD: src/sys/arch/mvme68k/include/pmap.h,v 1.9.2.2 2002/06/11 03:36:50 art Exp $	*/

#ifndef	_MACHINE_PMAP_H_
#define	_MACHINE_PMAP_H_

#include <m68k/pmap_motorola.h>

#ifdef	_KERNEL
void pmap_init_md(void);
#define	PMAP_INIT_MD()	pmap_init_md()
#endif

#endif	/* _MACHINE_PMAP_H_ */
