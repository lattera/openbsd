#	from @(#)Makefile	8.1 (Berkeley) 6/1/93
#	$Id$

PROG=	make_keypair
MAN=	make_keypair.8
CFLAGS+=-DKERBEROS -I${.CURDIR}/../register
DPADD=	${LIBKRB} ${LIBDES}
LDADD=	-lkdb -lkrb -ldes

.include <bsd.prog.mk>
