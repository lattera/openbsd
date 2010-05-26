#	$Id: Makefile,v 1.2 2007/12/28 17:22:32 reyk Exp $

.PATH:		${.CURDIR}/../snmpd

PROG=		snmpctl
SRCS=		log.c smi.c snmpctl.c parser.c

MAN=		snmpctl.8

LDADD=		-lutil
DPADD=		${LIBUTIL}
CFLAGS+=	-Wall -I${.CURDIR} -I${.CURDIR}/../snmpd
CFLAGS+=	-Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=	-Wmissing-declarations
CFLAGS+=	-Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=	-Wsign-compare -Wbounded

.include <bsd.prog.mk>
