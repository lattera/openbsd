#	from: @(#)Makefile	8.1 (Berkeley) 6/4/93
#	$Id$

PROG=	kpasswdd
SRCS=	kpasswdd.c des_rw.c
.PATH:	${.CURDIR}/../../usr.bin/rlogin
CFLAGS+=-DKERBEROS -I${.CURDIR}/../../usr.bin/passwd
DPADD=	${LIBKDB} ${LIBKRB} ${LIBDES}
LDADD=	-lkdb -lkrb -ldes
MAN=	kpasswdd.8
BINDIR=/usr/libexec

.include <bsd.prog.mk>
