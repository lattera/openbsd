#	from: @(#)Makefile	8.1 (Berkeley) 6/4/93
#	$Id: Makefile,v 1.1.1.1 1995/10/18 08:43:22 deraadt Exp $

PROG=	rshd
SRCS=	rshd.c
MAN=	rshd.8

.include <bsd.own.mk>

.if defined(KERBEROS)
SRCS+=	des_rw.c
.PATH:	${.CURDIR}/../../usr.bin/rlogin
CFLAGS+=-DKERBEROS
DPADD+=	${LIBKRB} ${LIBDES}
LDADD+=	-lkrb -ldes
.endif

.include <bsd.prog.mk>
