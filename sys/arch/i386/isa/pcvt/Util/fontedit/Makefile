#	$OpenBSD: src/sys/arch/i386/isa/pcvt/Util/fontedit/Attic/Makefile,v 1.3.6.1 2001/04/18 16:08:08 niklas Exp $

.if ${MACHINE} == "i386"
PROG=		fontedit
CLEANFILES+=	core.fontedit fontedit.core
.else
NOPROG=		yes
.endif
MAN1=	fontedit.1
MANSUBDIR=/i386

.include <bsd.prog.mk>
