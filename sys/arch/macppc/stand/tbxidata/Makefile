#       $OpenBSD: src/sys/arch/macppc/stand/tbxidata/Makefile,v 1.1.4.1 2001/10/31 03:01:17 nate Exp $

NOMACHINE=
FILES=	bsd.tbxi 
NOOBJ=	noobj

install:
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 ${FILES} \
	    ${DESTDIR}/usr/mdec

.include <bsd.prog.mk>
