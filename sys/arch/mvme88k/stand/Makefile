#	$OpenBSD: src/sys/arch/mvme88k/stand/Makefile,v 1.4.8.2 2004/02/19 10:49:08 niklas Exp $

.if ${MACHINE} == "mvme88k"
SUBDIR=  bugcrt libbug libsa libz wrtvid bootsd bootxx bootst netboot tftpboot
# not really written yet...
# SUBDIR+=	sboot
.endif

SUBDIR+=installboot

.include <bsd.subdir.mk>
