#	$OpenBSD: src/usr.bin/ssh/Makefile.inc,v 1.24.2.2 2004/08/19 22:37:30 brad Exp $

CFLAGS+=	-I${.CURDIR}/..

CDIAGFLAGS=	-Wall
#CDIAGFLAGS+=	-Werror
CDIAGFLAGS+=	-Wpointer-arith
CDIAGFLAGS+=	-Wno-uninitialized
#CDIAGFLAGS+=	-Wstrict-prototypes
CDIAGFLAGS+=	-Wmissing-prototypes
CDIAGFLAGS+=	-Wunused
#CDIAGFLAGS+=	-Wsign-compare
CDIAGFLAGS+=	-Wbounded
#CDIAGFLAGS+=	-Wshadow

#DEBUG=-g

#CFLAGS+=	-DSMARTCARD
#LDADD+=	-lsectok

.include <bsd.obj.mk>

.if exists(${.CURDIR}/../lib/${__objdir})
LDADD+=         -L${.CURDIR}/../lib/${__objdir} -lssh
DPADD+=         ${.CURDIR}/../lib/${__objdir}/libssh.a
.else
LDADD+=         -L${.CURDIR}/../lib -lssh
DPADD+=         ${.CURDIR}/../lib/libssh.a
.endif
