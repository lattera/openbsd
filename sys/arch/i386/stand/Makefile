#	$OpenBSD: src/sys/arch/i386/stand/Makefile,v 1.8.10.1 2001/04/18 16:08:37 niklas Exp $

.if ${MACHINE} == "i386"
SUBDIR=	etc libsa libz mbr
.endif
SUBDIR+= biosboot installboot boot

test-fd0:
	sudo mount /dev/fd0a /mnt
	sudo cp /usr/mdec/boot /mnt
	sudo /usr/mdec/installboot -v /mnt/boot /usr/mdec/biosboot fd0
	sudo umount /mnt

.include <bsd.subdir.mk>
