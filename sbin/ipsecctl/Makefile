#	$Id: Makefile,v 1.1.1.1 2005/02/02 21:31:54 hshoexer Exp $

PROG=	ipsecctl
MAN=	ipsecctl.8 ipsec.conf.5

SRCS=	ipsecctl.c pfkey.c parse.y

CFLAGS+= -Wall -I${.CURDIR}
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare

YFLAGS=

.include <bsd.prog.mk>
