#	$NetBSD: Makefile,v 1.20 1995/06/26 03:24:17 christos Exp $
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

LOADSZ=15

DEBUG=	-g -DDEBUG

OBJDUMP=/usr/i386-unknown-openbsd1.2/bin/objdump
AS=	/usr/i386-unknown-openbsd1.2/bin/as.new
AFLAGS+=-DLOADSZ=${LOADSZ}
CFLAGS+=-Werror -m386 -O2 -DNO_SPRINTF -DDO_BAD144
# no 16-bit-code flag yet
OBJDUMPFLAGS=-d

CPPFLAGS=${DEBUG} -I. -I${.CURDIR} -I$S -I${.CURDIR}/../..
# Uncomment this to make the boot block talk to a serial port.
#CPPFLAGS+=-DSERIAL=0

# SA lib stuff
SAREL=
.include "$S/lib/libsa/Makefile.inc"
CFLAGS+=-I$S/lib/libsa

# start.o should be first
OBJS=	start.o table.o boot.o asm.o bios.o io.o disk.o sys.o
CLEANFILES+=bootbios boot biosboot boot.sym machine ${MACHINE_ARCH}

all: machine-links biosboot bootbios


start.o: ${.CURDIR}/start.S
	@echo ${CC} ${CPPFLAGS} ${AFLAGS} ${.CURDIR}/start.S
	@${CC} -E ${CPPFLAGS} ${AFLAGS} ${.CURDIR}/start.S > $*.s
	@(${AS} -o $*.o $*.s; rm $*.s)

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
