#	$OpenBSD: src/usr.sbin/sendmail/makemap/Attic/Makefile,v 1.1.1.2 1998/07/12 17:09:38 millert Exp $
#	@(#)Makefile	8.1 (Berkeley) 6/7/93

.PATH:	${.CURDIR}/../src

PROG=	makemap
MAN=	makemap.8
CFLAGS+=-I${.CURDIR}/../src -DNEWDB -DNOT_SENDMAIL

SRCS=	makemap.c safefile.c

.include "../../Makefile.inc"
.include <bsd.prog.mk>
