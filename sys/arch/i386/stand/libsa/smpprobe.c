/*	$OpenBSD: src/sys/arch/i386/stand/libsa/smpprobe.c,v 1.3.8.2 2000/02/20 10:27:56 niklas Exp $	*/

/*
 * Copyright (c) 1997 Tobias Weingartner
 * All rights reserved.
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
 *	This product includes software developed by Tobias Weingartner.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/param.h>
#include <machine/biosvar.h>
#include <machine/mp.h>
#include <stand/boot/bootarg.h>
#include "libsa.h"

extern int debug;

extern u_int cnvmem;

static __inline int
mp_checksum(ptr, len)
	u_int8_t *ptr;
	int len;
{
	register int i, sum = 0;

#ifdef DEBUG
	printf("Checksum %p for %d\n", ptr, len);
#endif

	for (i = 0; i < len; i++)
		sum += *(ptr + i);

	return ((sum & 0xff) == 0);
}

struct mp_float *
mp_probefloat(ptr, len)
	u_int8_t *ptr;
	int len;
{
	struct mp_float *mpp;
	int i;

#ifdef DEBUG
	if (debug)
		printf("Checking %p for %d\n", ptr, len);
#endif
	for (i = 0, mpp = (struct mp_float *)ptr; i < len;
	    i += sizeof *mpp, mpp++) {
		if (bcmp(mpp->signature, MPF_SIGNATURE,
		    sizeof mpp->signature) == 0) {
#ifdef DEBUG
			if (debug)
				printf("Found possible MP signature at: %p\n",
				    mpp);
#endif
			if (mp_checksum(mpp, mpp->length * 16)) {
#ifdef DEBUG
				if (debug)
					printf("Found valid MP signature at: "
					    "%p\n", ptr);
#endif
				break;
			}
		}
	}

	return (i < len ? mpp : NULL);
}


void
smpprobe()
{
	struct mp_float *mp = NULL;

	/* Check EBDA */
	if (!(mp = mp_probefloat((void *)((*((u_int16_t*)0x40e)) * 16),
	    1024)) &&
		/* Check BIOS ROM 0xF0000 - 0xFFFFF */
	    !(mp = mp_probefloat((void *)(0xF0000), 0xFFFF)) &&
		/* Check last 1K of base RAM */
	    !(mp = mp_probefloat((void *)(cnvmem * 1024), 1024))) {
		/* No valid MP signature found */
#if DEBUG
		if (debug)
			printf("No valid MP signature found.\n");
#endif
		return;
	}

	/* Valid MP signature found */
	printf(" smp");
	addbootarg(BOOTARG_SMPINFO, mp->length * sizeof *mp, mp);

#if DEBUG
	if (debug)
		printf("Floating Structure:\n"
		    "\tSignature: %c%c%c%c\n"
		    "\tConfig at: %x\n"
		    "\tLength: %d\n"
		    "\tRev: 1.%d\n"
		    "\tFeature: %x %x %x %x %x\n",
		    mp->signature[0], mp->signature[1], mp->signature[2],
		    mp->signature[3], mp->pointer, mp->length, mp->revision,
		    mp->feature1, mp->feature2, mp->feature3, mp->feature4,
		    mp->feature5);
#endif
}

