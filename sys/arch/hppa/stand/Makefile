#	$OpenBSD: src/sys/arch/hppa/stand/Makefile,v 1.2.8.1 2003/05/19 21:43:13 tedu Exp $

.if ${MACHINE} == "hppa"
SUBDIR= libsa libkern libz
.endif
SUBDIR+= mkboot boot
.if ${MACHINE} == "hppa"
SUBDIR+= cdboot
.endif

.include <bsd.subdir.mk>
