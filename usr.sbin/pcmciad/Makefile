# $Id: Makefile,v 1.3 1996/08/21 06:48:20 tholo Exp $

SRCS=	pcmciad.c pcmcia_conf.c
.PATH:  ${.CURDIR}/../../sys/dev/pcmcia

CFLAGS+=-Wmissing-prototypes -I${.CURDIR}/../../sys
PROG=	pcmciad
MAN=	pcmciad.8

pcmcia_conf.o: pcmcia_conf.c
	${COMPILE.c} -D_KERNEL $<
clean::
	rm -f a.out [Ee]rrs mklog core *.core pcmciad pcmcia_conf.o pcmciad.o


.include <bsd.prog.mk>
