#	$OpenBSD: src/usr.bin/ssh/Makefile.inc,v 1.12.2.4 2001/05/07 21:09:25 jason Exp $

CFLAGS+=	-I${.CURDIR}/..

CFLAGS+=	-Wall

.include <bsd.obj.mk>

.if exists(${.CURDIR}/../lib/${__objdir})
LDADD+=         -L${.CURDIR}/../lib/${__objdir} -lssh
DPADD+=         ${.CURDIR}/../lib/${__objdir}/libssh.a
.else
LDADD+=         -L${.CURDIR}/../lib -lssh
DPADD+=         ${.CURDIR}/../lib/libssh.a
.endif
