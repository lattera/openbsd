#	$OpenBSD: src/sys/arch/macppc/stand/mbr/Makefile,v 1.1.4.1 2001/10/31 03:01:17 nate Exp $

CLEANFILES= mbr msdos1mb.gz

all: mbr msdos1mb.gz

mbr: mbr.uu
	uudecode ${.CURDIR}/mbr.uu

msdos1mb.gz: msdos1mb.gz.uu
	uudecode ${.CURDIR}/msdos1mb.gz.uu

realinstall:
	${INSTALL} -c -o ${BINOWN} -g ${BINGRP} -m 444 mbr msdos1mb.gz \
		${DESTDIR}/usr/mdec


.include <bsd.prog.mk>
