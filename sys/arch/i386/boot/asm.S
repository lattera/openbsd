/*	$OpenBSD: src/sys/arch/i386/boot/Attic/asm.S,v 1.2.2.3 1996/09/26 12:15:45 mickey Exp $	*/
/*	$NetBSD: asm.S,v 1.12 1995/03/12 00:10:53 mycroft Exp $	*/

/*
 * Changes by Michael Shalayeff 1996
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

CR0_PE		=	0x1

	.data
	.globl	_ourseg
_ourseg:
	.long	0
	.text

/*
 *
 */

/* Conventional GDT indexes. */
BOOT_CS_INDEX	=	3
BOOT_CS16_INDEX	=	5
BOOT_DS_INDEX	=	4

	.globl	boot2
boot2:
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
	shll	$4, %eax

	/* fix up GDT entries for bootstrap */
#define FIXUP(gdt_index) \
	addr32; \
	movl	%ax, _C_LABEL(Gdt)+(8*gdt_index)+2; /* actually movw %ax */ \
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

	ljmp	$0x18, $_C_LABEL(boot)

	hlt
	cli

/*
 * real_to_prot()
 * 	transfer from real mode to protected mode.
 */
ENTRY(real_to_prot)
	# guarantee that interrupt is disabled when in prot mode
	cli

	# load the gdtr
	addr32
	data32
	lgdt	_C_LABEL(Gdtr)

	# set the PE bit of CR0
	movl	%cr0, %eax
	data32
	orl	$CR0_PE, %eax
	movl	%eax, %cr0 

	# make intrasegment jump to flush the processor pipeline and
	# reload CS register
	data32
	ljmp	$0x18, $1f
1:
	# we are in USE32 mode now
	# set up the protected mode segment registers : DS, SS, ES
	movl	$0x20, %eax
	movl	%ax, %ds
	movl	%ax, %ss
	movl	%ax, %es

	ret

/*
 * prot_to_real()
 * 	transfer from protected mode to real mode
 */
ENTRY(prot_to_real)
	# set up a dummy stack frame for the second seg change.
	# Adjust the intersegment jump instruction following
	# the clearing of protected mode bit.
	# This is self-modifying code, but we need a writable
	# code segment, and an intersegment return does not give us that.
	movl    _ourseg, %eax
	movw    %ax, xreal-2

	# Change to use16 mode.
	ljmp	$0x28, $x16
x16:
	# clear the PE bit of CR0
	movl	%cr0, %eax
	data32
	andl 	$~CR0_PE, %eax
	movl	%eax, %cr0

	# make intersegment jmp to flush the processor pipeline
	# using the fake stack frame set up earlier
	# and reload CS register
	# Here we have an 16 bits intersegment jump.
	.byte 0xea
	.word xreal
	.word 0

xreal:
	# we are in real mode now
	# set up the real mode segment registers : DS, SS, ES
	movl	%cs, %ax
	movl	%ax, %ds
	movl	%ax, %ss
	movl	%ax, %es

	sti
	data32
	ret

/*
 * startprog(phyaddr)
 *	start the program on protected mode where phyaddr is the entry point
 */
ENTRY(startprog)
	pushl	%ebp
	movl	%esp, %ebp

	# get things we need into registers
	movl	8(%ebp), %ecx		# entry offset 
	movl	12(%ebp), %eax		# &argv

	# make a new stack at 0:0x90000 (big segs)
	movl	$0x10, %ebx
	movw	%bx, %ss
	movl	$0x90000, %ebx
	movl	%ebx, %esp
	
	# push some number of args onto the stack
	pushl	28(%eax)		# argv[7] = cnvmem
	pushl	32(%eax)		# argv[8] = extmem
	pushl	16(%eax)		# argv[4] = esym
	pushl	$0			# nominally a cyl offset in the boot.
	pushl	8(%eax)			# argv[2] = bootdev
	pushl	4(%eax)			# argv[1] = howto
	pushl	$0			# dummy 'return' address
	
	# push on our entry address
	pushl	$0x8			# segment
	pushl	%ecx

	# convert over the other data segs
	movl	$0x10, %ebx
	movl	%bx, %ds
	movl	%bx, %es

	# convert the PC (and code seg)
	lret

/*
 * pbzero(dst, cnt)
 *	where dst is a virtual address and cnt is the length
 */
ENTRY(pbzero)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%es
	pushl	%edi

	cld

	# set %es to point at the flat segment
	movl	$0x10, %eax
	movl	%ax, %es

	movl	8(%ebp), %edi		# destination
	movl	12(%ebp), %ecx		# count
	xorl	%eax, %eax		# value

	rep
	stosb

	popl	%edi
	popl	%es
	popl	%ebp
	ret

/*
 * pcpy(src, dst, cnt)
 *	where src is a virtual address and dst is a physical address
 */
ENTRY(pcpy)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%es
	pushl	%esi
	pushl	%edi

	cld

	# set %es to point at the flat segment
	movl	$0x10, %eax
	movl	%ax, %es

	movl	8(%ebp), %esi		# source
	movl	12(%ebp), %edi		# destination
	movl	16(%ebp), %ecx		# count

	rep
	movsb

	popl	%edi
	popl	%esi
	popl	%es
	popl	%ebp
	ret

#ifdef CHECKSUM
/*
 * cksum(src, cnt)
 *	where src is a virtual address and cnt is the length
 */
ENTRY(cksum)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%es
	pushl	%edi

	cld

	# set %es to point at the flat segment
	movl	$0x10, %eax
	movl	%ax, %es

	movl	8(%ebp), %edi		# destination
	movl	12(%ebp), %ecx		# count
	shrl	$2, %ecx
	xorl	%edx, %edx		# value

1:	es
	lodsl
	xorl	%eax, %edx
	loop	1b

	movl	%edx, %eax

	popl	%edi
	popl	%es
	popl	%ebp
	ret
#endif

#if 0
ENTRY(getword)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%es

	# set %es to point at the flat segment
	movl	$0x10, %eax
	movl	%ax, %es

	movl	8(%ebp), %eax
	es
	movl	(%eax), %edx

	movl	%edx, %eax

	popl	%es
	popl	%ebp
	ret
#endif
