#	$OpenBSD: src/usr.bin/ssh/scard/Attic/Makefile,v 1.2.2.1 2001/09/27 00:15:50 miod Exp $

.PATH:		${.CURDIR}/..

CARDLET=	Ssh.bin
DATADIR=	/usr/libdata/ssh

all: ${CARDLET}

clean:
	rm -f ${CARDLET}

install: ${CARDLET}
	install -c -m ${LIBMODE} -o ${LIBOWN} -g ${LIBGRP} \
	    ${CARDLET} ${DESTDIR}${DATADIR}

Ssh.bin: ${.CURDIR}/Ssh.bin.uu
	uudecode ${.CURDIR}/$@.uu

.include <bsd.prog.mk>
