# $Id: Makefile,v 1.1.1.1 1995/10/18 08:43:11 deraadt Exp $

LIB=	skey
SRCS=	skeylogin.c skeysubr.c put.c
HDRS=	skey.h
#CFLAGS+= -DSKEY_MDX_DEFAULT=4

includes:
	@cd ${.CURDIR}; for i in $(HDRS); do \
	    j="cmp -s $$i ${DESTDIR}/usr/include/`basename $$i` || \
		install -c -o ${BINOWN} -g ${BINGRP} -m 444 $$i \
		${DESTDIR}/usr/include"; \
	    echo $$j; \
	    eval "$$j"; \
	done

.include <bsd.lib.mk>
