#	from: @(#)Makefile	5.6 (Berkeley) 9/27/90
#	$Id: Makefile,v 1.1.1.1 1995/10/18 08:46:03 deraadt Exp $

PROG=	rsh
SRCS=	rsh.c
BINOWN=	root
BINMODE=4555

.include <bsd.own.mk>	# For KERBEROS

.if defined(KERBEROS)
SRCS+=	des_rw.c
.PATH:	${.CURDIR}/../rlogin
CFLAGS+=-DKERBEROS
SRCS+=	krcmd.c kcmd.c
LDADD+=	-lkrb -ldes
DPADD+=	${LIBKRB} ${LIBDES}
.endif

.include <bsd.prog.mk>
