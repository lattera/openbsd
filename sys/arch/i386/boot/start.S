/*	$OpenBSD: src/sys/arch/i386/boot/Attic/start.S,v 1.4.2.4 1996/10/23 08:37:52 mickey Exp $	*/
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

#include <machine/asm.h>

BOOTSEG		=	0x0100	# boot placement
BOOTSTACK	=	0xfffc	# boot stack
BOOTBIOS	=	0x7c0	# segment where we loaded
BOOTRELOC	=	0x7a0	# segment where to relocate
SIGNATURE	=	0xaa55	# MBR signature
NUMPART		=	4	# number of partitions in partition table
PARTSZ		=	16	# each partition table entry is 16 bytes
BSDPART		=	0xA5	# OpenBSD partition
OLDBSDPART	=	0xA5	# BSD partition
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
CHAR_B		=	':'	# booting

#ifdef DEBUG
#define DBGMSG(msg) \
	movb    $msg, %al ; \
	call    chr
#else /* !DEBUG */
#define DBGMSG(msg)
#endif /* !DEBUG */

	.file	"start.S"
	.code16
	.globl	boot2

ENTRY(boot1)
#ifndef DOSREAD
	cli	# disable interrupts w/o stack
	/*
	 * os-bs and other mbrs pass us part entry in %ds:%si
	 */
	# start (aka boot1) is loaded at 0x0:0x7c00 but we want 0x7c0:0
	# ljmp to the next instruction to adjust %cs
	ljmp $BOOTBIOS, $start1

start1:
	# set up stack(%ss:%esp)
	movw	$BOOTSEG, %ax
	movw	%ax, %ss
	movl	$BOOTSTACK, %esp

	/*** set up %es, (where we will load boot2 to) ***/
	movw	%ax, %es

	sti	# we have stack, do ints

#ifdef SERIAL
	# Initialize the serial port to 9600 baud, 8N1.
	pushw	%dx
	movw	$0x00e3, %ax
	movw	SERIAL, %dx
	int	$0x14
	popw	%dx
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
	pushw	%cs
	popw	%ds
	movw	$0x0001, %cx	# cyl 0, sector 1
	xorb	%dh, %dh	# head
	jmp	3f
#endif
#ifndef	NOHD
1:
	DBGMSG(CHAR_H)
	# movb	4(%si), %al	# partition type
	.byte   0x8a, 0x44, 0x04
	cmpb	$BSDPART, %al
	je	2f		# we are not a MBR

	# set up %ds
	pushw	%cs
	popw	%ds
	xorw	%bx, %bx
	# cmpw	$SIGNATURE, (%bx)
	.byte	0x81, 0xbf
	.word	signature
	.word	SIGNATURE
	je	1f
	DBGMSG(CHAR_P)

1:
	/* find the first active partition */
	DBGMSG(CHAR_L)
	movw	$pt, %si
	movw	$NUMPART, %cx
1:
	# movb	(%si), %al
	.byte	0x8a, 0x44, 0x00
	cmpb	$BOOTABLE, %al
	je	2f
	addw	$PARTSZ, %si
	loop	1b
	movw	$enoboot, %si
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
	pushw	%cs
	popw	%ds
	cmpb	$BSDPART, %al
	jne	1f	/* foreign partition */
#endif
3:
	call	load

	DBGMSG(CHAR_G)

	# jmp to the second stage boot loader (boot2).
	# After ljmp, %cs is BOOTBIOS and boot1 (512 bytes) will be used
	# as an internal buffer "intbuf".

	ljmp	$BOOTSEG, $boot2
	/* not reached */

#ifndef	NOHD
1:
	# relocate the code to leave the space for next stage
	DBGMSG(CHAR_R)
	pushw	%es
	pushw	%cx
	pushw	$BOOTRELOC
	popw	%es
	xorw	%si, %si
	xorw	%di, %di
	movw	$0x100, %cx
	cld
	rep
	movsw
	popw	%cx
	popw	%es
	# jump to the relocated code
	ljmp $BOOTRELOC, $start2

start2:
	call	load

	DBGMSG(CHAR_D)
	# jump to the new code (%ds:%si is at he right point)
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
	movw	$0x200 | LOADSZ, %ax	/* number of blocks */
	xorw	%bx, %bx		/* put it at BOOTBIOS:0 */
	int	$0x13
	jc	read_error
	ret

#
#	read_error
#

read_error:
	movw	$eread, %si
err_stop:
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
	pushw	%ax

#ifndef SERIAL
	pushw	%bx
	movb	$0x0e, %ah
	movw	$0x0001, %bx
	int	$0x10
	popw	%bx
#else
	pushw	%dx
	movb	$0x01, %ah
	movw	SERIAL, %dx
	int	$0x14
	popw	%dx
#endif
	popw	%ax
	ret

/*
 * Display string
 */
message:
#ifndef	DEBUG
	pushw	%ax
#endif

nextb:
	cld
	lodsb			# load a byte into %al
	testb	%al, %al
	jz	done
	call	chr

	jmp	nextb

done:
#ifndef	DEBUG
	popw	%ax
#endif
	ret
#else	/* !DOSREAD */
	movb	$0xff, %dl
	jmp	boot2
#endif /* DOSREAD */

#if 0
/* Conventional GDT indexes. */
BOOT_CS_INDEX	=	3
BOOT_CS16_INDEX	=	5
BOOT_DS_INDEX	=	4

boot2:
	pushw	%cs
	pushw	%cs
	pushw	%cs
	popw	%ds
	popw	%es
	xorl	%eax, %eax
	popw	%ax
	movl	%eax, _C_LABEL(ourseg)
	shll	$4, %eax

	/* fix up GDT entries for bootstrap */
#define FIXUP(gdt_index) \
	movw	%ax, _C_LABEL(Gdt)+(8*gdt_index)+2; \
	movb	%bl, _C_LABEL(Gdt)+(8*gdt_index)+4

	shldl	$16, %eax, %ebx
	FIXUP(BOOT_CS_INDEX)
	FIXUP(BOOT_CS16_INDEX)
	FIXUP(BOOT_DS_INDEX)

	/* fix up GDT pointer */
	addl	$_C_LABEL(Gdt), %eax
	movl	%eax, _C_LABEL(Gdtr)+2

	/* change to protected mode */
	call	_C_LABEL(real_to_prot)

	.code32

	/* clear the bss */
	movl	$_C_LABEL(edata), %edi
	movl	$_C_LABEL(end), %ecx
	subl	%edi, %ecx
	xorb	%al, %al
	rep
	stosb

	movzbl	%dl, %edx	/* discard head (%dh) and random high bits */
	pushl	%edx
	call	_C_LABEL(boot)
#endif
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


	.asciz	"boot2"

