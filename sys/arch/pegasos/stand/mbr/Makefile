#	$OpenBSD: src/sys/arch/pegasos/stand/mbr/Attic/Makefile,v 1.1.2.1 2004/02/19 10:49:56 niklas Exp $

CLEANFILES= mbr

all: mbr

mbr:
	dd if=/dev/zero of=mbr bs=512 count=1

realinstall:
	${INSTALL} -c -o ${BINOWN} -g ${BINGRP} -m 444 mbr ${DESTDIR}/usr/mdec


.include <bsd.prog.mk>
