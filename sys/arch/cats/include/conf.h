/*	$OpenBSD: src/sys/arch/cats/include/Attic/conf.h,v 1.1.2.1 2004/02/19 10:48:02 niklas Exp $	*/
/*	$NetBSD: conf.h,v 1.8 2002/02/10 12:26:03 chris Exp $	*/

#ifndef _CATS_CONF_H
#define	_CATS_CONF_H

/*
 * CATS specific device includes go in here
 */
#include "fcom.h"

#define	CONF_HAVE_PCI
#define	CONF_HAVE_USB
#define	CONF_HAVE_SCSIPI
#define	CONF_HAVE_WSCONS

#include <arm/conf.h>

#endif	/* _CATS_CONF_H */
