/* $OpenBSD: src/sys/arch/i386/i386/apicvec.s,v 1.1.2.10 2004/03/23 23:02:19 niklas Exp $ */	
/* $NetBSD: apicvec.s,v 1.1.2.2 2000/02/21 21:54:01 sommerfeld Exp $ */	

/*-
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by RedBack Networks Inc.
 *
 * Author: Bill Sommerfeld
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
	
#include <machine/i82093reg.h>	
#include <machine/i82489reg.h>	

#ifdef __ELF__
#define XINTR(name) Xintr/**/name
#else
#define XINTR(name) _Xintr/**/name
#endif

#define	IDTVEC(name)	ALIGN_TEXT; .globl X/**/name; X/**/name:

#ifdef MULTIPROCESSOR
IDTVEC(recurse_lapic_ipi)
	pushfl
	pushl	%cs
	pushl	%esi
	pushl	$0		
	pushl	$T_ASTFLT
	INTRENTRY		
IDTVEC(resume_lapic_ipi)
	cli
	jmp	1f
IDTVEC(intr_lapic_ipi)
	.globl	XINTR(ipi)
XINTR(ipi):
	pushl	$0		
	pushl	$T_ASTFLT
	INTRENTRY		
	MAKE_FRAME		
	movl	$0,_C_LABEL(local_apic)+LAPIC_EOI
	movl	CPUVAR(ILEVEL),%ebx
	cmpl	$IPL_IPI,%ebx
	jae	2f
1:
#ifdef notyet
	incl	CPUVAR(IDEPTH)
#endif
	movl	$IPL_IPI,CPUVAR(ILEVEL)
        sti			/* safe to take interrupts.. */
	pushl	%ebx
	call	_C_LABEL(i386_ipi_handler)
	jmp	_C_LABEL(Xdoreti)
2:
	orl	$(1 << LIR_IPI),CPUVAR(IPENDING)
	sti
	INTRFASTEXIT

#ifdef notyet
#if defined(DDB)
IDTVEC(intrddbipi)
1:
	str	%ax
	GET_TSS
	movzwl	(%eax),%eax
	GET_TSS
	pushl	%eax
	movl	$0xff,_C_LABEL(lapic_tpr)
	movl	$0,_C_LABEL(local_apic)+LAPIC_EOI
	sti
	call	_C_LABEL(ddb_ipi_tss)
	addl	$4,%esp
	movl	$0,_C_LABEL(lapic_tpr)
	iret
	jmp	1b
#endif /* DDB */
#endif
	
	/*
	 * Interrupt from the local APIC timer.
	 */
IDTVEC(recurse_lapic_ltimer)
	pushfl
	pushl	%cs
	pushl	%esi
	pushl	$0		
	pushl	$T_ASTFLT
	INTRENTRY		
IDTVEC(resume_lapic_ltimer)
	cli
	jmp	1f
IDTVEC(intr_lapic_ltimer)
	.globl	XINTR(ltimer)
XINTR(ltimer):			
	pushl	$0		
	pushl	$T_ASTFLT
	INTRENTRY		
	MAKE_FRAME		
	movl	$0,_C_LABEL(local_apic)+LAPIC_EOI
	movl	CPUVAR(ILEVEL),%ebx
	cmpl	$IPL_CLOCK,%ebx
	jae	2f
1:
	pushl	%ebx
#ifdef notyet
	incl	CPUVAR(IDEPTH)
#endif
	movl	$IPL_CLOCK,CPUVAR(ILEVEL)
	sti
#ifdef MULTIPROCESSOR
	call	_C_LABEL(i386_softintlock)
#endif
	movl	%esp,%eax
	pushl	%eax
	call	_C_LABEL(lapic_clockintr)
	addl	$4,%esp		
#ifdef MULTIPROCESSOR
	call	_C_LABEL(i386_softintunlock)
#endif
	jmp	_C_LABEL(Xdoreti)
2:
	orl	$(1 << LIR_TIMER),CPUVAR(IPENDING)
	sti
	INTRFASTEXIT

	.globl	XINTR(softclock), XINTR(softnet), XINTR(softtty)
XINTR(softclock):
	pushl	$0		
	pushl	$T_ASTFLT
	INTRENTRY		
	MAKE_FRAME		
	movl	CPUVAR(ILEVEL),%ebx
	pushl	%ebx
	movl	$IPL_SOFTCLOCK,CPUVAR(ILEVEL)
#ifdef notyet
	incl	CPUVAR(IDEPTH)
#endif
	andl	$~(1<<SIR_CLOCK),CPUVAR(IPENDING)
	movl	$0,_C_LABEL(local_apic)+LAPIC_EOI
	sti
#ifdef MULTIPROCESSOR
	call	_C_LABEL(i386_softintlock)
#endif
	call	_C_LABEL(softclock)
#ifdef MULTIPROCESSOR
	call	_C_LABEL(i386_softintunlock)
#endif
#ifdef notyet
	decl	CPUVAR(IDEPTH)
#endif
	jmp	_C_LABEL(Xdoreti)
	
#define DONETISR(s, c) \
	.globl  _C_LABEL(c)	;\
	testl	$(1 << s),%edi	;\
	jz	1f		;\
	call	_C_LABEL(c)	;\
1:

XINTR(softnet):
	pushl	$0		
	pushl	$T_ASTFLT
	INTRENTRY		
	MAKE_FRAME		
	movl	CPUVAR(ILEVEL),%ebx
	pushl	%ebx
	movl	$IPL_SOFTNET,CPUVAR(ILEVEL)
#ifdef notyet
	incl	CPUVAR(IDEPTH)
#endif
	andl	$~(1<<SIR_NET),CPUVAR(IPENDING)
	movl	$0,_C_LABEL(local_apic)+LAPIC_EOI	
	sti
#ifdef MULTIPROCESSOR
	call	_C_LABEL(i386_softintlock)
#endif
	xorl	%edi,%edi
	xchgl	_C_LABEL(netisr),%edi
#include <net/netisr_dispatch.h>
#ifdef MULTIPROCESSOR
	call	_C_LABEL(i386_softintunlock)
#endif
#ifdef notyet
	decl	CPUVAR(IDEPTH)
#endif
	jmp	_C_LABEL(Xdoreti)
#undef DONETISR

XINTR(softtty):	
	pushl	$0		
	pushl	$T_ASTFLT
	INTRENTRY		
	MAKE_FRAME		
	movl	CPUVAR(ILEVEL),%ebx
	pushl	%ebx
	movl	$IPL_SOFTTTY,CPUVAR(ILEVEL)
#ifdef notyet
	incl	CPUVAR(IDEPTH)
#endif
	andl	$~(1<<SIR_TTY),CPUVAR(IPENDING)
	movl	$0,_C_LABEL(local_apic)+LAPIC_EOI	
	sti
#ifdef MULTIPROCESSOR
	call	_C_LABEL(i386_softintlock)
#endif
	call	_C_LABEL(comsoft)
#ifdef MULTIPROCESSOR
	call	_C_LABEL(i386_softintunlock)
#endif
#ifdef notyet
	decl	CPUVAR(IDEPTH)
#endif
	jmp	_C_LABEL(Xdoreti)

#if NIOAPIC > 0

#define voidop(num)

	/*
	 * I/O APIC interrupt.
	 * We sort out which one is which based on the value of 
	 * the processor priority register.
	 *
	 * XXX no stray interrupt mangling stuff..
	 * XXX use cmove when appropriate.
	 */
	
#define APICINTR(name, num, early_ack, late_ack, mask, unmask, level_mask) \
IDTVEC(recurse_/**/name/**/num)						;\
	pushfl								;\
	pushl	%cs							;\
	pushl	%esi							;\
	subl	$4,%esp							;\
	pushl	$T_ASTFLT		/* trap # for doing ASTs */	;\
	INTRENTRY							;\
IDTVEC(resume_/**/name/**/num)						\
/*	movl	$IREENT_MAGIC,TF_ERR(%esp)	*/			;\
/*	movl	%ebx,%esi	*/					;\
/*	movl	CPUVAR(ISOURCES) + (num) * 4, %ebp	*/		;\
/*	movl	IS_MAXLEVEL(%ebp),%ebx	*/				;\
	jmp	1f							;\
/*IDTVEC(intr_-**-name-**-num)*/					;\
XINTR(_/**/name/**/num):						\
	pushl	$0							;\
	pushl	$T_ASTFLT						;\
	INTRENTRY							;\
	MAKE_FRAME							;\
/*	movl	CPUVAR(ISOURCES) + (num) * 4, %ebp	*/		;\
	mask(num)			/* mask it in hardware */	;\
	early_ack(num)			/* and allow other intrs */	;\
/*	movl	IS_MAXLEVEL(%ebp),%ebx	*/				;\
	movl	CPUVAR(ILEVEL),%esi					;\
/*	cmpl	%ebx,%esi	*/					;\
/*	jae	10f		*/	/* currently masked; hold it */	;\
/*	incl	MY_COUNT+V_INTR	*/	/* statistical info */		;\
1:									;\
	pushl	%esi							;\
	movl	_C_LABEL(lapic_ppr),%eax				;\
	movl	%eax,CPUVAR(ILEVEL)					;\
	sti								;\
	orl	$num,%eax						;\
	incl	_C_LABEL(apic_intrcount)(,%eax,4)			;\
/*	incl	CPUVAR(IDEPTH)	*/					;\
	movl	_C_LABEL(apic_intrhand)(,%eax,4),%ebx /* chain head */	;\
	testl	%ebx,%ebx						;\
	jz	8f			/* oops, no handlers.. */	;\
7:	movl	IH_ARG(%ebx),%eax	/* get handler arg */		;\
	testl	%eax,%eax						;\
	jnz	4f							;\
	movl	%esp,%eax		/* 0 means frame pointer */	;\
4:									 \
	pushl	%eax							;\
	call	*IH_FUN(%ebx)		/* call it */			;\
	addl	$4,%esp			/* toss the arg */		;\
	orl	%eax,%eax		/* should it be counted? */	;\
	jz	5f			/* no, skip it */		;\
	incl	IH_COUNT(%ebx)		/* count the intrs */		;\
5:	movl	IH_NEXT(%ebx),%ebx	/* next handler in chain */	;\
	testl	%ebx,%ebx						;\
	jnz	7b							;\
	UNLOCK_KERNEL							;\
6:	cli								;\
	unmask(num)			/* unmask it in hardware */	;\
	late_ack(num)							;\
	sti								;\
	jmp	_C_LABEL(Xdoreti)					;\
8:	pushl	$num							;\
	call	_C_LABEL(isa_strayintr)					;\
	addl	$4,%esp							;\
	jmp	6b							;\
10:									;\
	orb	$IRQ_BIT(num),CPUVAR(IPENDING) + IRQ_BYTE(num)		;\
	sti								;\
	INTRFASTEXIT
	
APICINTR(ioapic,0, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,1, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,2, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,3, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,4, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,5, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,6, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,7, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,8, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,9, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,10, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,11, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,12, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,13, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,14, voidop, ioapic_asm_ack, voidop, voidop, voidop)
APICINTR(ioapic,15, voidop, ioapic_asm_ack, voidop, voidop, voidop)

	.globl	_C_LABEL(Xintr_ioapic0),_C_LABEL(Xintr_ioapic1)
	.globl	_C_LABEL(Xintr_ioapic2),_C_LABEL(Xintr_ioapic3)
	.globl	_C_LABEL(Xintr_ioapic4),_C_LABEL(Xintr_ioapic5)
	.globl	_C_LABEL(Xintr_ioapic6),_C_LABEL(Xintr_ioapic7)
	.globl	_C_LABEL(Xintr_ioapic8),_C_LABEL(Xintr_ioapic9)
	.globl	_C_LABEL(Xintr_ioapic10),_C_LABEL(Xintr_ioapic11)
	.globl	_C_LABEL(Xintr_ioapic12),_C_LABEL(Xintr_ioapic13)
	.globl	_C_LABEL(Xintr_ioapic14),_C_LABEL(Xintr_ioapic15)
	.globl _C_LABEL(apichandler)

_C_LABEL(apichandler):	
	.long	_C_LABEL(Xintr_ioapic0),_C_LABEL(Xintr_ioapic1)
	.long	_C_LABEL(Xintr_ioapic2),_C_LABEL(Xintr_ioapic3)
	.long	_C_LABEL(Xintr_ioapic4),_C_LABEL(Xintr_ioapic5)
	.long	_C_LABEL(Xintr_ioapic6),_C_LABEL(Xintr_ioapic7)
	.long	_C_LABEL(Xintr_ioapic8),_C_LABEL(Xintr_ioapic9)
	.long	_C_LABEL(Xintr_ioapic10),_C_LABEL(Xintr_ioapic11)
	.long	_C_LABEL(Xintr_ioapic12),_C_LABEL(Xintr_ioapic13)
	.long	_C_LABEL(Xintr_ioapic14),_C_LABEL(Xintr_ioapic15)
#endif
