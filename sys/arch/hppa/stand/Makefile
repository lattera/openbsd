#	$OpenBSD: src/sys/arch/hppa/stand/Makefile,v 1.1.1.1.8.2 2003/05/13 19:41:04 ho Exp $

.if ${MACHINE} == "hppa"
SUBDIR= libsa libkern libz
.endif
SUBDIR+= mkboot boot
.if ${MACHINE} == "hppa"
SUBDIR+= cdboot
.endif

.include <bsd.subdir.mk>
