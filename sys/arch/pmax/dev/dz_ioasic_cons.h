/*	$OpenBSD: src/sys/arch/pmax/dev/Attic/dz_ioasic_cons.h,v 1.1.4.1 2001/04/18 16:12:39 niklas Exp $	*/
/*	$NetBSD: dc_ioasic_cons.h,v 1.1 1996/09/25 20:48:56 jonathan Exp $	*/

#ifdef _KERNEL
#ifndef _DZ_IOASIC_CONS_H
#define _DZ_IOASIC_CONS_H

/*
 * Following declaratios for console code.
 * XXX should be redesigned to expose less driver internals.
 */
int dz_ioasic_consinit __P((dev_t dev));

#endif	/* _DZ_IOASIC_CONS_H */
#endif	/* _KERNEL */
