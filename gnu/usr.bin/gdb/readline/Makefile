# $Id: Makefile,v 1.1.1.1 1995/10/18 08:40:14 deraadt Exp $

LIB=		readline
NOPROFILE=
NOPIC=

CFLAGS+=	-DVOID_SIGHANDLER -DVI_MODE -DHAVE_UNISTD_H \
		-I$(.CURDIR)/../include/
SRCS=		funmap.c history.c keymaps.c readline.c vi_mode.c tilde.c

install:

.include <bsd.lib.mk>
