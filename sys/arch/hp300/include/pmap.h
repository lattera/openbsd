/*	$OpenBSD: src/sys/arch/hp300/include/pmap.h,v 1.11.2.2 2002/06/11 03:35:20 art Exp $	*/

#ifndef	_MACHINE_PMAP_H_
#define	_MACHINE_PMAP_H_

#include <m68k/pmap_motorola.h>

#ifdef	_KERNEL
void pmap_init_md(void);
#define	PMAP_INIT_MD()	pmap_init_md()
#endif

#endif	/* _MACHINE_PMAP_H_ */
