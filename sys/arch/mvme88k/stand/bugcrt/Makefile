#    $Id: Makefile,v 1.1 1996/02/04 03:36:28 build Exp build $

CFLAGS+=-I${.CURDIR}/../../include 

.include "${MACHINE_ARCH}/Makefile.inc"

.PATH:  ${.CURDIR}/${MACHINE_ARCH}

OBJS=bugcrt.o

CLEANFILES+=a.out

all:	${OBJS}

bugcrt.o: bugcrt.c
	${CC} ${CFLAGS} -c ${.ALLSRC}
	${LD} -x -r ${.TARGET}
	mv a.out ${.TARGET}

install:

lint tags:

.include <bsd.prog.mk>
