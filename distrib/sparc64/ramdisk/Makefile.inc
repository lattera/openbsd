#	$OpenBSD: src/distrib/sparc64/ramdisk/Attic/Makefile.inc,v 1.1 2001/09/18 14:27:32 jason Exp $
#	$OpenBSD: src/distrib/sparc64/ramdisk/Attic/Makefile.inc,v 1.1 2001/09/18 14:27:32 jason Exp $

IMAGESIZE?=		5352
GZIP?=			gzip
GZIPFLAGS?=		-9v
GZIPEXT?=		.gz
MAKEFLOPPY=		1
RAWLABEL=
BOOT=			${DESTDIR}/usr/mdec/boot
INSTALLBOOT=		${DESTDIR}/usr/mdec/installboot
