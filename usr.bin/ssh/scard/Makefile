#	$OpenBSD: src/usr.bin/ssh/scard/Attic/Makefile,v 1.2.4.2 2002/03/09 00:22:15 miod Exp $

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
