/*	$OpenBSD: src/sys/arch/i386/boot/Attic/start.S,v 1.4.2.5 1996/10/28 12:14:29 mickey Exp $	*/
/*	$NetBSD: start.S,v 1.12 1995/01/18 17:34:18 mycroft Exp $	*/

/*
 * Copyright (c) 1996 Michael Shalayeff
 */

/* Copyright (c) 1996 VaX#n8 (vax@linkdead.paranoia.com)
 * last edited 9 July 1996
 * many thanks to Erich Boleyn (erich@uruk.org) for putting up with
 * all my questions, and for his work on GRUB
 * You may use this code or fragments thereof in a manner consistent
 * with the other copyrights as long as you retain my pseudonym and
 * this copyright notice in the file.
 */

/*-
 *
 * Ported to boot 386BSD by Julian Elischer (julian@tfs.com) Sept 1992
 *
 * Mach Operating System
 * Copyright (c) 1992, 1991 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */

/*
  Copyright 1988, 1989, 1990, 1991, 1992 
   by Intel Corporation, Santa Clara, California.

                All Rights Reserved

Permission to use, copy, modify, and distribute this software and
its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appears in all
copies and that both the copyright notice and this permission notice
appear in supporting documentation, and that the name of Intel
not be used in advertising or publicity pertaining to distribution
of the software without specific, written prior permission.

INTEL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL INTEL BE LIABLE FOR ANY SPECIAL, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN ACTION OF CONTRACT,
NEGLIGENCE, OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

	.file	"start.S"

#include <machine/asm.h>
#include <machine/specialreg.h>

#define addr32  .byte 0x67
#define data32  .byte 0x66

BOOTSEG		=	0x0100	# boot placement
BOOTSTACK	=	0xfffc	# boot stack
BOOTBIOS	=	0x7c0	# segment where we loaded
BOOTRELOC	=	0x7a0	# segment where to relocate
SIGNATURE	=	0xaa55	# MBR signature
NUMPART		=	4	# number of partitions in partition table
PARTSZ		=	16	# each partition table entry is 16 bytes
BSDPART		=	0xA6	# OpenBSD partition
OLDBSDPART	=	0xA5	# {386,Net,Free}BSD partition
BOOTABLE	=	0x80	# bootable partition
CHAR_S		=	'S'	# started
CHAR_2		=	'2'	# loading 2nd stage boot
CHAR_G		=	'G'	# jumping to it
CHAR_F		=	'F'	# booting from floppy
CHAR_H		=	'H'	# booting from HD
CHAR_L		=	'L'	# looking up bootable partition
CHAR_R		=	'R'	# relocating
CHAR_D		=	'D'	# booting from non-BSD partition
CHAR_P		=	'P'	# partition table corrupted

#ifdef DEBUG
#define DBGMSG(msg)		\
	movb    $msg, %al;	\
	data32;			\
	call    chr
#else /* !DEBUG */
#define DBGMSG(msg)
#endif /* !DEBUG */

	.text

	.globl	_boot1
	.globl	_boot2
	.globl	_boot

ENTRY(boot1)
#ifndef DOSREAD
	cli	# disable interrupts w/o stack
	/*
	 * os-bs and other mbrs pass us part entry in %ds:%si
	 */
	# start (aka boot1) is loaded at 0x0:0x7c00 but we want 0x7c0:0
	# ljmp to the next instruction to adjust %cs
	data32
	ljmp $BOOTBIOS, $1f

1:
	# set up stack(%ss:%esp)
	movl	$BOOTSEG, %ax
	movl	%ax, %ss
	data32
	movl	$BOOTSTACK, %esp

	/*** set up %es, (where we will load boot2 to) ***/
	movl	%ax, %es

	sti	# we have stack, do ints

#ifdef SERIAL
	# Initialize the serial port to 9600 baud, 8N1.
	pushl	%edx
	movl	$0x00e3, %ax
	movl	SERIAL, %dx
	int	$0x14
	popl	%edx
#endif

#if	!defined(NOFD) && !defined(NOHD)
	DBGMSG(CHAR_S)

	# bootstrap passes us drive number in %dl
	testb	$0x80, %dl
	jnz	1f
#endif
#ifndef	NOFD
	DBGMSG(CHAR_F)
	# set up %ds
	pushl	%cs
	popl	%ds
	movl	$0x0001, %cx	# cyl 0, sector 1
	xorb	%dh, %dh	# head
	jmp	3f
#endif
#ifndef	NOHD
1:
	DBGMSG(CHAR_H)
	# movb	4(%si), %al	# partition type
	.byte   0x8a, 0x44, 0x04
	cmpb	$BSDPART, %al
	je	2f		# we are MBR !

	# set up %ds
	pushl	%cs
	popl	%ds
	xorl	%bx, %bx
	# cmpw	$SIGNATURE, (%bx)
	.byte	0x81, 0xbf
	.word	signature
	.word	SIGNATURE
	je	1f
	DBGMSG(CHAR_P)

1:
	/* find the first active partition */
	DBGMSG(CHAR_L)
	data32
	movl	$pt, %esi
	data32
	movl	$NUMPART, %ecx
1:
	# movb	(%si), %al
	.byte	0x8a, 0x44, 0x00
	cmpb	$BOOTABLE, %al
	je	2f
	addl	$PARTSZ, %esi
	loop	1b
	data32
	movl	$enoboot, %esi
	jmp	err_stop
#endif

#ifndef	NOHD
2:
	DBGMSG(CHAR_2)
	# movb	1(%si), %dh	# head
	.byte   0x8a, 0x74, 0x01
	# movw	2(%si), %cx	# sect, cyl
	.byte   0x8b, 0x4c, 0x02
	# movb	4(%si), %al	# partition type
	.byte   0x8a, 0x44, 0x04

	# set up %ds
	pushl	%cs
	popl	%ds
	cmpb	$BSDPART, %al
	jne	1f	/* foreign partition */
#endif
3:
	data32
	call	load

	DBGMSG(CHAR_G)

	# jmp to the second stage boot loader (boot2).
	# After ljmp, %cs is BOOTBIOS and boot1 (512 bytes) will be used
	# as an internal buffer "intbuf".

	data32
	ljmp	$BOOTSEG, $_boot2
	/* not reached */

#ifndef	NOHD
1:
	# relocate the code to leave the space for next stage
	DBGMSG(CHAR_R)
	pushl	%es
	data32
	pushl	%ecx
	data32
	movl	$BOOTRELOC, %eax
	movl	%ax, %es
	xorl	%si, %si
	xorl	%di, %di
	data32
	movl	$0x100, %ecx
	cld
	rep
	movsl
	data32
	popl	%ecx
	popl	%es
	# jump to the relocated code
	data32
	ljmp $BOOTRELOC, $1f

1:
	data32
	call	load

	DBGMSG(CHAR_D)
	# jump to the new code (%ds:%si is at he right point)
	data32
	ljmp	$0, $BOOTBIOS << 4
	/* not reached */
#endif

/*
# BIOS call "INT 0x13 Function 0x2" to read sectors from disk into memory
#       Call with       %ah = 0x2
#                       %al = number of sectors
#                       %ch = cylinder
#                       %cl = sector
#                       %dh = head
#                       %dl = drive (0x80 for hard disk, 0x0 for floppy disk)
#                       %es:%bx = segment:offset of buffer
#       Return:
#                       %al = 0x0 on success; err code on failure
*/
load:
	data32
	movl	$0x200 | LOADSZ, %eax	/* number of blocks */
	xorl	%bx, %bx		/* put it at BOOTBIOS:0 */
	int	$0x13
	jc	read_error
	data32
	ret

#
#	read_error
#

read_error:
	movl	$eread, %si
err_stop:
	data32
	call	message
	jmp	stop

#
#	message: write the error message in %ds:%si to console
#
	.globl	chr
chr:
/*
#ifndef SERIAL
# BIOS call "INT 10H Function 0Eh" to write character to console
#	Call with	%ah = 0x0e
#			%al = character
#			%bh = page
#			%bl = foreground color
#else
# BIOS call "INT 14H Function 01h" to write character to console
#	Call with	%ah = 0x01
#			%al = character
#			%dx = port number
#endif
*/
	data32
	pushl	%ax

#ifndef SERIAL
	data32
	pushl	%bx
	movb	$0x0e, %ah
	data32
	movl	$0x001f, %bx
	int	$0x10
	data32
	popl	%bx
#else
	data32
	pushl	%dx
	movb	$0x01, %ah
	data32
	movl	SERIAL, %dx
	int	$0x14
	data32
	popl	%dx
#endif
	data32
	popl	%ax
	data32
	ret

/*
 * Display string
 */
message:
	data32
	pushl	%ax
	cld
1:
	lodsb			# load a byte into %al
	testb	%al, %al
	jz	1f
	data32
	call	chr
	jmp	1b
1:
	data32
	popl	%ax
	data32
	ret

#else	/* !DOSREAD */
	movb	$0xff, %dl
	jmp	$_boot2
#endif /* DOSREAD */

stop:
	cli
	hlt


/* error messages */

#ifndef	DOSREAD
eread:	.asciz		"Read error\r\n"
enoboot: .asciz		"No partition to boot\r\n"
prompt:	.asciz		"OpenBSD?"

endofcode:
	nop

/* throw in a partition in case we are block0 as well */
/* flag, head, sec, cyl, typ, ehead, esect, ecyl, start, len */
	. = _C_LABEL(boot1) + 0x1be	# starting address of partition table
pt:
	.byte	0x0,0,0,0,0,0,0,0
	.long	0,0
	.byte	0x0,0,0,0,0,0,0,0
	.long	0,0
	.byte	0x0,0,0,0,0,0,0,0
	.long	0,0
	.byte	BOOTABLE,0,1,0,BSDPART,255,255,255
	.long	0,50000
/* the last 2 bytes in the sector 0 contain the signature */
	. = _C_LABEL(boot1) + 0x1fe
signature:
	.short	SIGNATURE
	. = _C_LABEL(boot1) + 0x200
#endif
	.globl	_disklabel
_disklabel:
	. = _C_LABEL(boot1) + 0x400	


	.ascii	"\nboot2:\n"

	.data
	.globl  _bootdev
_bootdev:	.long   0
	.globl  _boothowto
_boothowto:	.long   0
	.globl  _cyloffset
_cyloffset:	.long   0

/*
 *
 */

/* Conventional GDT indexes. */
BOOT_CS_INDEX	=	3
BOOT_CS16_INDEX	=	5
BOOT_DS_INDEX	=	4

	.globl	_ourseg
	.globl	_Gdt
	.globl	_Gdtr
	.globl	_edata
	.globl	_end

	.text
ENTRY(boot2)
	pushl	%cs
	pushl	%cs
	pushl	%cs
	popl	%ds
	popl	%es
	data32
	xorl	%eax, %eax
	popl	%ax
	data32
	addr32
	movl	%eax, _C_LABEL(ourseg)
	data32
	shll	$4, %eax

	/* fix up GDT entries for bootstrap */
#define FIXUP(gdt_index) \
	addr32; \
	movl	%eax, _C_LABEL(Gdt)+(8*gdt_index)+2; /* actualy movw %ax */ \
	addr32; \
	movb	%bl, _C_LABEL(Gdt)+(8*gdt_index)+4

	data32
	shldl	$16, %eax, %ebx
	FIXUP(BOOT_CS_INDEX)
	FIXUP(BOOT_CS16_INDEX)
	FIXUP(BOOT_DS_INDEX)

	/* fix up GDT pointer */
	data32
	addl	$_C_LABEL(Gdt), %eax
	data32
	addr32
	movl	%eax, _C_LABEL(Gdtr)+2

	/* change to protected mode */
	data32
	call	_C_LABEL(real_to_prot)

	/* clear the bss */
	movl	$_C_LABEL(edata), %edi
	movl	$_C_LABEL(end), %ecx
	subl	%edi, %ecx
	xorb	%al, %al
	rep
	stosb

	movzbl	%dl, %edx	/* discard head (%dh) and random high bits */
	pushl	%edx
	call	_boot

	hlt
	cli

