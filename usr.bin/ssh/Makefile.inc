#	$OpenBSD: src/usr.bin/ssh/Makefile.inc,v 1.11.2.2 2001/03/12 15:44:06 jason Exp $

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
