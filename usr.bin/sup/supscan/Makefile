#	$Id$

.PATH:	${.CURDIR}/../src

PROG=	supscan
NOMAN=
SRCS=	supscan.o stree.o scan.o \
	atoo.o errmsg.o expand.o ffilecopy.o filecopy.o nxtarg.o \
	path.o quit.o run.o salloc.o skipto.o vprintf.o netcryptvoid.c
LDADD+=	-lutil
DPADD+=	${LIBUTIL}
BINOWN=	root
BINGRP=	daemon
BINMODE=555
BINDIR=	/usr/bin
CFLAGS+=-I${.CURDIR}/../src

.include <bsd.prog.mk>
