#	$OpenBSD: src/sys/arch/amd64/stand/Makefile,v 1.1.2.1 2004/06/05 23:09:25 niklas Exp $

.if ${MACHINE} == "amd64"
SUBDIR=	etc mbr
.endif
SUBDIR+= biosboot installboot boot pxeboot

.include <bsd.subdir.mk>
