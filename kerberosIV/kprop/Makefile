#	$Id: Makefile,v 1.2 1997/06/29 07:21:16 deraadt Exp $

PROG=	kprop
LDADD+=	-lkrb -ldes
DPADD+=	${LIBKRB} ${LIBDES}
MAN=	kprop.8
CFLAGS+=-Wall

.include <bsd.prog.mk>
