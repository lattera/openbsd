/*	$OpenBSD: src/sys/arch/i386/boot/Attic/boot.c,v 1.7.2.3 1996/10/28 12:14:26 mickey Exp $	*/

/*
 * Copyright (c) 1996 Michael Shalayeff
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
 *	This product includes software developed by Michael Shalayeff.
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

#include <libsa.h>
#include <biosdev.h>

char *names[] = {
	"/boot", "/oboot", "/boot.old"
};

void
boot(drive)
	int drive;
{
	register int currname = 0;
		
	gateA20(1);

	do {
		/*
		 * As a default set it to the first partition of the first
		 * floppy or hard drive	
		 */
		part = 0;
		unit = drive&0x7f;
		maj = (drive&0x80 ? 0 : 2);         /* a good first bet */

		if (currname == NENTS(names))
			currname = 0;

		printf("loading %s: ", names[currname]);
		exec (names[currname], (caddr_t)0x10000, boothowto);
		printf("\n");
		currname++;

	} while (1);
}

#ifdef	EXEC_DEBUG
int
getchar()
{
	asm("hlt");
	return 0;
}
#endif
