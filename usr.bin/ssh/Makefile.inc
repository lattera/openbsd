#	$OpenBSD: src/usr.bin/ssh/Makefile.inc,v 1.12.2.3 2001/03/21 19:46:21 jason Exp $

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
