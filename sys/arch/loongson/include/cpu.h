/* $OpenBSD: src/sys/arch/loongson/include/cpu.h,v 1.3 2012/04/21 12:20:30 miod Exp $ */
/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell and Rick Macklem.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	Copyright (C) 1989 Digital Equipment Corporation.
 *	Permission to use, copy, modify, and distribute this software and
 *	its documentation for any purpose and without fee is hereby granted,
 *	provided that the above copyright notice appears in all copies.
 *	Digital Equipment Corporation makes no representations about the
 *	suitability of this software for any purpose.  It is provided "as is"
 *	without express or implied warranty.
 *
 *	from: @(#)cpu.h	8.4 (Berkeley) 1/4/94
 */

#ifdef	_KERNEL

#define	Mips_SyncCache(ci)			\
	Loongson2_SyncCache((ci))
#define	Mips_InvalidateICache(ci, va, l)	\
	Loongson2_InvalidateICache((ci), (va), (l))
#define	Mips_SyncDCachePage(ci, va, pa)		\
	Loongson2_SyncDCachePage((ci), (va), (pa))
#define	Mips_HitSyncDCache(ci, va, l)	\
	Loongson2_HitSyncDCache((ci), (va), (l))
#define	Mips_IOSyncDCache(ci, va, l, h)	\
	Loongson2_IOSyncDCache((ci), (va), (l), (h))
#define	Mips_HitInvalidateDCache(ci, va, l)	\
	Loongson2_HitInvalidateDCache((ci), (va), (l))

#endif	/* _KERNEL */

#include <mips64/cpu.h>
