#	from @(#)Makefile	8.1 (Berkeley) 6/1/93
#	$Id$

PROG=	ksrvtgt
CFLAGS+=-DKERBEROS
DPADD=	${LIBKRB} ${LIBDES}
LDADD=	-lkrb -ldes
BINDIR=	/usr/bin

.include <bsd.prog.mk>
