/*	$OpenBSD: src/sys/arch/i386/include/intr.h,v 1.4.4.5 2001/10/28 22:41:19 niklas Exp $	*/
/*	$NetBSD: intr.h,v 1.5 1996/05/13 06:11:28 mycroft Exp $	*/

/*
 * Copyright (c) 1996 Charles M. Hannum.  All rights reserved.
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
 *	This product includes software developed by Charles M. Hannum.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _I386_INTR_H_
#define _I386_INTR_H_

/* Interrupt priority `levels'; not mutually exclusive. */
#define	IPL_NONE	0x00			/* nothing */
#define	IPL_SOFTCLOCK	(NRSVIDT + 0x10)	/* timeouts */
#define	IPL_SOFTNET	(NRSVIDT + 0x20)	/* protocol stacks */
#define	IPL_BIO		(NRSVIDT + 0x30)	/* block I/O */
#define	IPL_NET		(NRSVIDT + 0x40)	/* network */
#define	IPL_SOFTTTY	(NRSVIDT + 0x50)	/* delayed terminal handling */
#define	IPL_TTY		(NRSVIDT + 0x60)	/* terminal */
#define	IPL_IMP		(NRSVIDT + 0x70)	/* memory allocation */
#define	IPL_AUDIO	(NRSVIDT + 0x80)	/* audio */
#define	IPL_CLOCK	(NRSVIDT + 0x90)	/* clock */
#define	IPL_HIGH	(NRSVIDT + 0xa0)	/* everything, except... */
#define	IPL_IPI		(NRSVIDT + 0xb0)	/* interprocessor interrupt */
#define NIPL		12

/* Interrupt sharing types. */
#define	IST_NONE	0	/* none */
#define	IST_PULSE	1	/* pulsed */
#define	IST_EDGE	2	/* edge-triggered */
#define	IST_LEVEL	3	/* level-triggered */

/* Soft interrupt masks. */
#define	SIR_CLOCK	31
#define	SIR_CLOCKMASK	((1 << SIR_CLOCK))
#define	SIR_NET		30
#define	SIR_NETMASK	((1 << SIR_NET) | SIR_CLOCKMASK)
#define	SIR_TTY		29
#define	SIR_TTYMASK	((1 << SIR_TTY) | SIR_CLOCKMASK)
#define	SIR_ALLMASK	(SIR_CLOCKMASK | SIR_NETMASK | SIR_TTYMASK)

#ifndef _LOCORE

#ifdef MULTIPROCESSOR
#include <machine/i82489reg.h>
#include <machine/i82489var.h>
#endif

extern volatile u_int32_t lapic_tpr;
volatile u_int32_t ipending;

#ifndef MULTIPROCESSOR
volatile u_int32_t astpending;
#endif

int imask[NIPL];
int iunmask[NIPL];

#define CPSHIFT 4
#define IMASK(level) imask[level >> CPSHIFT]
#define IUNMASK(level) iunmask[level >> CPSHIFT]

extern void Xspllower __P((void));

static __inline int splraise __P((int));
static __inline int spllower __P((int));
static __inline void splx __P((int));
static __inline void softintr __P((int, int));

/*
 * Add a mask to cpl, and return the old value of cpl.
 */
static __inline int
splraise(ncpl)
	register int ncpl;
{
	register int ocpl = lapic_tpr;

	if (ncpl > ocpl)
		lapic_tpr = ncpl;
	return (ocpl);
}

/*
 * Restore a value to cpl (unmasking interrupts).  If any unmasked
 * interrupts are pending, call Xspllower() to process them.
 */
static __inline void
splx(ncpl)
	register int ncpl;
{
	lapic_tpr = ncpl;
	if (ipending & IUNMASK(ncpl))
		Xspllower();
}

/*
 * Same as splx(), but we return the old value of spl, for the
 * benefit of some splsoftclock() callers.
 */
static __inline int
spllower(ncpl)
	register int ncpl;
{
	register int ocpl = lapic_tpr;

	splx(ncpl);

/* XXX - instead of splx() call above.
	lapic_tpr = ncpl;
	if (ipending & IUNMASK(ncpl))
		Xspllower();
*/
	return (ocpl);
}

/*
 * Hardware interrupt masks
 */
#define	splbio()	splraise(IPL_BIO)
#define	splnet()	splraise(IPL_NET)
#define	spltty()	splraise(IPL_TTY)
#define	splaudio()	splraise(IPL_AUDIO)
#define	splclock()	splraise(IPL_CLOCK)
#define	splstatclock()	splhigh()

/*
 * Software interrupt masks
 *
 * NOTE: spllowersoftclock() is used by hardclock() to lower the priority from
 * clock to softclock before it calls softclock().
 */
#define	spllowersoftclock()	spllower(IPL_SOFTCLOCK)
#define	splsoftclock()		splraise(IPL_SOFTCLOCK)
#define	splsoftnet()		splraise(IPL_SOFTNET)
#define	splsofttty()		splraise(IPL_SOFTTTY)

/*
 * Miscellaneous
 */
#define	splimp()	splraise(IPL_IMP)
#define	splvm()		splraise(IPL_IMP)
#define	splhigh()	splraise(IPL_HIGH)
#define	spl0()		spllower(IPL_NONE)

/*
 * Software interrupt registration
 *
 * We hand-code this to ensure that it's atomic.
 */
static __inline void
softintr(sir, vec)
	register int sir;
	register int vec;
{
	__asm __volatile("orl %0,_ipending" : : "ir" (sir));
#ifdef MULTIPROCESSOR
	i82489_writereg(LAPIC_ICRLO,
	    vec | LAPIC_DLMODE_FIXED | LAPIC_LVL_ASSERT | LAPIC_DEST_SELF);
#endif
}

#define	setsoftast()	(astpending = 1)
#define	setsoftclock()	softintr(1 << SIR_CLOCK,IPL_SOFTCLOCK)
#define	setsoftnet()	softintr(1 << SIR_NET,IPL_SOFTNET)
#define	setsofttty()	softintr(1 << SIR_TTY,IPL_SOFTTTY)

#define I386_IPI_HALT	0x00000001
#define I386_IPI_TLB	0x00000002
#define I386_IPI_FPSAVE	0x00000004

/* the following are for debugging.. */
#define I386_IPI_GMTB	0x00000010
#define I386_IPI_NYCHI	0x00000020

#define I386_NIPI	6

struct cpu_info;
void i386_send_ipi (struct cpu_info *, int);
void i386_broadcast_ipi (int);
void i386_ipi_handler (void);

#endif /* !_LOCORE */

#endif /* !_I386_INTR_H_ */
