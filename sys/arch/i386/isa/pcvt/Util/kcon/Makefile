#	$OpenBSD: src/sys/arch/i386/isa/pcvt/Util/kcon/Attic/Makefile,v 1.3.6.1 2001/04/18 16:08:11 niklas Exp $

.if ${MACHINE} == "i386"
PROG=		kcon
DEVICE=		/dev/ttyC0
CFLAGS+= 	-I${.CURDIR}/../keycap -DKEYB_DEVICE=\"${DEVICE}\"

# the -Lfoo could be omitted if libkeycap.a were installed before
# making those programs here

LDADD =       -L${.CURDIR}/../keycap -L${.CURDIR}/../keycap/obj -lkeycap
.else
NOPROG=yes
.endif

MAN1=	kcon.1
MANSUBDIR=/i386

.include <bsd.prog.mk>
