/*	$OpenBSD: src/games/battlestar/misc.c,v 1.7 2004/07/10 07:26:22 deraadt Exp $	*/
/*	$NetBSD: misc.c,v 1.3 1995/03/21 15:07:37 cgd Exp $	*/

/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 */

#ifndef lint
#if 0
static char sccsid[] = "@(#)misc.c	8.2 (Berkeley) 4/28/95";
#else
static char rcsid[] = "$OpenBSD: src/games/battlestar/misc.c,v 1.7 2004/07/10 07:26:22 deraadt Exp $";
#endif
#endif /* not lint */

#include "extern.h"

/* for beenthere, injuries */
int
card(const char *array, int size)
{
	const char   *end = array + size;
	int     i = 0;

	while (array < end)
		if (*array++)
			i++;
	return (i);
}

int
ucard(const unsigned int *array)
{
	int     j = 0, n;

	for (n = 0; n < NUMOFOBJECTS; n++)
		if (TestBit(array, n))
			j++;
	return (j);
}
