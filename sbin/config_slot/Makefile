# $Id: Makefile,v 1.4 1996/03/24 01:59:04 jtk Exp $

SRCS	= config_slot.c pcmcia_conf.c
VPATH	= ${.CURDIR}/../../sys/dev/pcmcia
#dumpreg.c

CFLAGS+= -g  -O2 -Wmissing-prototypes -I${.CURDIR}/../../sys
PROG=config_slot
NOMAN=

pcmcia_conf.o: pcmcia_conf.c
	$(CC) $(CFLAGS) -D_KERNEL -c $<

.include <bsd.prog.mk>
# DO NOT DELETE THIS LINE -- make depend depends on it.
