#	$OpenBSD: src/sys/arch/i386/stand/biosboot/Makefile,v 1.18.4.2 2003/05/13 19:42:09 ho Exp $

MAN=	biosboot.8

.if ${MACHINE} == "i386"
PROG=	biosboot
SRCS=	biosboot.S
LD=ld
LDFLAGS=-nostdlib -Ttext 0 -N -x -Bstatic
INSTALL_STRIP=
SADIR=	${.CURDIR}/..

${PROG}: $(OBJS) $(DPADD)
	@rm -f $(PROG)
	$(LD) $(LDFLAGS) -o $(PROG) $(OBJS) $(LDADD)
	@size $(PROG)

CPPFLAGS+=-DLOADADDR=$(LOADADDR) -DLINKADDR=$(LINKADDR) -DBOOTMAGIC=$(BOOTMAGIC)
CPPFLAGS+=${DEBUGFLAGS}
#AFLAGS+=-Wa,-a
.else
NOPROG=
.endif

.include <bsd.prog.mk>

