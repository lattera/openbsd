#	$OpenBSD: src/sys/arch/i386/boot/Attic/Makefile,v 1.3.2.4 1996/10/28 12:14:24 mickey Exp $
#
# Ported to boot 386BSD by Julian Elischer (julian@tfs.com)
# September 1992
#
# Permission to use, copy, modify and distribute this software and its
# documentation is hereby granted, provided that both the copyright
# notice and this permission notice appear in all copies of the
# software, derivative works or modified versions, and any portions
# thereof, and that both notices appear in supporting documentation.
#
# CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
# CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
# ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
#
# Carnegie Mellon requests users of this software to return to
#
#  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
#  School of Computer Science
#  Carnegie Mellon University
#  Pittsburgh PA 15213-3890
#
# any improvements or extensions that they make and grant Carnegie Mellon
# the rights to redistribute these changes.
#

S=	${.CURDIR}/../../..

LOADSZ=16

DEBUG=	-g -DDEBUG
NO_NET=no_net

CFLAGS+=-Wall -Werror -m386 -O2
# strip off uneeded stuff
CFLAGS+=-DNO_WRITE -DNO_READDIR -DNO_LSEEK -DNO_STAT -DINSECURE -DSTRIPPED
# use kbd io
CFLAGS+=-Dputc=kbd_putc

CPPFLAGS=${DEBUG} -I${.CURDIR} -I$S -I${.CURDIR}/../stand/libsa
# Uncomment this to make the boot block talk to a serial port.
#CPPFLAGS+=-DSERIAL=0

# SA lib stuff
SAREL=
.include "$S/lib/libsa/Makefile.inc"
CFLAGS+=-I$S/lib/libsa

# start.o should be first
OBJS=	start.o table.o boot.o conf.o dev_subr.o
CLEANFILES+=bootbios boot biosboot boot.sym machine ${MACHINE_ARCH}

all: machine-links biosboot bootbios


start.o: ${.CURDIR}/start.S
	${CC} -E ${CPPFLAGS} ${AFLAGS} -DLOADSZ=${LOADSZ} ${.CURDIR}/start.S > tmp.s
	${AS} -o $*.o tmp.s && rm tmp.s

machine-links:
	ln -fs ${.CURDIR}/../include machine
	ln -fs ${.CURDIR}/../include ${MACHINE_ARCH}

boot:	${OBJS} ${SALIB} version.o
	${LD} -Bstatic -e _boot1 -N -T 0 -o ${.TARGET} ${OBJS} ${SALIB} version.o
	cp boot boot.sym
	@strip boot
	@size boot | \
	awk 'BEGIN {getline} \
		{if($$1+$$2>${LOADSZ}*512){\
			print "BOOT BLOCKS ARE TOO BIG"; exit(1)}}'
	@sh ${.CURDIR}/rmaouthdr boot boot.tmp
	@mv -f boot.tmp boot
	@ls -l boot

biosboot:	boot
	dd if=boot of=biosboot count=1

bootbios:	boot
	dd if=boot of=bootbios skip=1

${DESTDIR}/usr/mdec/bootsd:	bootbios
	cp bootbios ${DESTDIR}/usr/mdec/bootsd

${DESTDIR}/usr/mdec/sdboot:	biosboot
	cp biosboot ${DESTDIR}/usr/mdec/sdboot

${DESTDIR}/usr/mdec/bootwd:	${DESTDIR}/usr/mdec/bootsd
	ln -f ${DESTDIR}/usr/mdec/bootsd ${DESTDIR}/usr/mdec/bootwd

${DESTDIR}/usr/mdec/wdboot:	${DESTDIR}/usr/mdec/sdboot
	ln -f ${DESTDIR}/usr/mdec/sdboot ${DESTDIR}/usr/mdec/wdboot

${DESTDIR}/usr/mdec/bootfd:	${DESTDIR}/usr/mdec/bootsd
	ln -f ${DESTDIR}/usr/mdec/bootsd ${DESTDIR}/usr/mdec/bootfd

${DESTDIR}/usr/mdec/fdboot:	${DESTDIR}/usr/mdec/sdboot
	ln -f ${DESTDIR}/usr/mdec/sdboot ${DESTDIR}/usr/mdec/fdboot

sd:	${DESTDIR}/usr/mdec/bootsd ${DESTDIR}/usr/mdec/sdboot
wd:	${DESTDIR}/usr/mdec/bootwd ${DESTDIR}/usr/mdec/wdboot
fd:	${DESTDIR}/usr/mdec/bootfd ${DESTDIR}/usr/mdec/fdboot

install: wd sd fd

depend::

clean::
	rm -f a.out [Ee]rrs mklog core *.core
	rm -f ${OBJS} ${LOBJS} ${CLEANFILES}

.include <bsd.prog.mk>
