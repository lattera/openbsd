/*	$OpenBSD: src/sys/arch/i386/boot/Attic/start.S,v 1.4.2.1 1996/09/17 09:34:13 mickey Exp $	*/
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
#define	addr32	.byte 0x67
#define	data32	.byte 0x66

BOOTSEG		=	0x0100	# boot will be loaded here (below 640K)
BOOTSTACK	=	0xfffc	# boot stack
BOOTBIOS	=	0x7c0	# segment where we loaded
BOOTRELOC	=	0x5c0	# segment where to relocate
SIGNATURE	=	0xaa55	# MBR signature
PARTSTART	=	0x1be	# starting address of partition table
NUMPART		=	4	# number of partitions in partition table
PARTSZ		=	16	# each partition table entry is 16 bytes
BSDPART		=	0xA5	# value of boot_ind, means bootable partition
BOOTABLE	=	0x80	# value of boot_ind, means bootable partition
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
#define DBGMSG(msg) \
	movb    $msg, %al ; \
	call    chr
#else /* !DEBUG */
#define DBGMSG(msg)
#endif /* !DEBUG */

	.text
	.globl	start

ENTRY(boot1)
start:
#ifndef DOSREAD
	cli	# disable interrupts w/o stack
	/*
	 * os-bs and other mbrs pass us part entry in %ds:%si
	 */
	# start (aka boot1) is loaded at 0x0:0x7c00 but we want 0x7c0:0
	# ljmp to the next instruction to adjust %cs
	data32
	ljmp $BOOTBIOS, $start1

start1:
	# set up stack(%ss:%esp)
	movl	$BOOTSEG, %ax
	movl	%ax, %ss
	data32
	movl	$BOOTSTACK, %esp

	sti	# we have stack, do ints

	/*** set up %es, (where we will load boot2 to) ***/
	movl	%ax, %es

#ifdef SERIAL
	# Initialize the serial port to 9600 baud, 8N1.
	pushl	%dx
	movl	$0x00e3, %ax
	movl	SERIAL, %dx
	int	$0x14
	popl	%dx
#endif

#if	!defined(NOFD) && !defined(NOHD)
	DBGMSG(CHAR_S)

	# bootstrap passes us drive number in %dl
	testb	$0x80, %dl
	jnz	hd
#endif
#ifndef	NOFD
fd:
	DBGMSG(CHAR_F)
	# set up %ds
	pushl	%cs
	popl	%ds
	data32
	movl	$0x0001, %ecx	# cyl 0, sector 1
	movb	$0x00, %dh	# head
	jmp	doload
#endif
#ifndef	NOHD
hd:
	DBGMSG(CHAR_H)
	# movl	%si, %bx
	# movb	1(%ebx), %dh	# head
	.byte   0x8a, 0x74, 0x01
	# movl	2(%ebx), %ecx	# sect, cyl (+ 2 bytes junk in top word)
	.byte   0x8b, 0x4c, 0x02
	# movb	4(%ebx), %al	# partition type
	.byte   0x8a, 0x44, 0x04
	cmpb	$BSDPART, %al
	# set up %ds
	pushl	%cs
	popl	%ds
	je	doload		# we are not a MBR

	movl	$signature, %esi
	cmpl	$SIGNATURE, (%esi)
	je	pt_ok
	DBGMSG(CHAR_P)

pt_ok:
	/* find the first active partition */
	DBGMSG(CHAR_L)
	data32
	movl	$PARTSTART, %esi
	data32
	movl	$NUMPART, %ecx
again:
	addr32
	cmpb	$BOOTABLE, (%esi)
	je	found
	data32
	addl	$PARTSZ, %esi
	loop	again
	data32
	movl	$enoboot, %esi
	jmp	err_stop
#endif

#ifndef	NOHD
found:
	DBGMSG(CHAR_2)
	addr32
	movb	1(%esi), %dh /* head */
	addr32
	movl	2(%esi), %ecx /*sect, cyl (+ 2 bytes junk in top word) */

	cmpb	$BSDPART, 4(%esi)
	jne	foreign	/* foreign partition */
#endif
doload:
	call	load

	DBGMSG(CHAR_G)

	# ljmp to the second stage boot loader (boot2).
	# After ljmp, %cs is BOOTSEG and boot1 (512 bytes) will be used
	# as an internal buffer "intbuf".

	data32
	ljmp	$BOOTSEG, $_C_LABEL(boot2)
	/* not reached */

#ifndef	NOHD
foreign:
	# relocate the code to leave the space for next stage
	DBGMSG(CHAR_R)
	data32
	pushl	%ecx
	movl	$BOOTRELOC, %ax
	movl	%ax, %es
	data32
	xorl	%esi, %esi
	data32
	xorl	%edi, %edi
	movl	$0x100, %cx
	cld
	rep
	movsl
	data32
	popl	%ecx
	# jump to the new code
	data32
	ljmp $BOOTRELOC, $start2

start2:
	/*** set up %es, (where we will load boot2 to) ***/
	pushl	$BOOTBIOS
	popl	%es

	data32
	call	load

	DBGMSG(CHAR_D)
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
	movl	$0x200 | LOADSZ, %ax	/* number of blocks */
	xorl	%bx, %bx		/* put it at BOOTSEG:0 */
	int	$0x13
	jc	read_error
	ret

#
#	read_error
#

read_error:
	data32
	movl	$eread, %esi
err_stop:
	call	message
	jmp	stop

#
#	message: write the error message in %ds:%esi to console
#

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
	pushl	%eax

#ifndef SERIAL
	data32
	pushl	%ebx
	movb	$0x0e, %ah
	data32
	movl	$0x0001, %ebx
	int	$0x10
	data32
	popl	%ebx
#else
	data32
	pushl	%edx
	movb	$0x01, %ah
	movl	SERIAL, %edx
	int	$0x14
	data32
	popl	%edx
#endif
	data32
	popl	%eax
	ret

/*
 * Display string
 */
message:
	data32
	pushl	%eax

nextb:
	cld
	addr32
	lodsb			# load a byte into %al
	testb	%al, %al
	data32
	jz	done
	call	chr

	data32
	jmp	nextb

done:
	data32
	popl	%eax
	ret
#else	/* !DOSREAD */
	movb	$0xff, %dl
	jmp	_C_LABEL(boot2)
#endif /* DOSREAD */

/* Conventional GDT indexes. */
BOOT_CS_INDEX	=	3
BOOT_CS16_INDEX	=	5
BOOT_DS_INDEX	=	4

ENTRY(boot2)
	data32
	xorl	%eax, %eax
	movl	%cs, %ax
	movl	%ax, %ds
	movl	%ax, %es
	addr32
	data32
	movl	%eax, _C_LABEL(ourseg)
	data32
	shll	$4, %eax

	/* fix up GDT entries for bootstrap */
#define FIXUP(gdt_index) \
	addr32; \
	movl	%eax, _C_LABEL(Gdt)+(8*gdt_index)+2;  /* actually movw %ax */ \
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
	addr32
	data32
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
	call	_C_LABEL(boot)

stop:
	cli
	hlt


/* error messages */

#ifndef	DOSREAD
eread:	.asciz		"Read error\r\n"
enoboot: .asciz		"No partition to boot\r\n"
endofcode:
/* throw in a partition in case we are block0 as well */
/* flag, head, sec, cyl, typ, ehead, esect, ecyl, start, len */
	. = _C_LABEL(boot1) + PARTSTART
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
