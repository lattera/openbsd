/*	$NetBSD: boot.c,v 1.29 1995/12/23 17:21:27 perry Exp $	*/

/*
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

#include <sys/param.h>
#include <sys/exec.h>
#include "boot.h"
#include <sys/reboot.h>

struct exec head;
int argv[9];
#ifdef CHECKSUM
int cflag;
#endif
char *name;
char *names[] = {
	"/boot", "/oboot", "/boot.old",
};
#define NUMNAMES	(sizeof(names)/sizeof(char *))

static void loadprog __P((int howto));

extern char version[];
extern int end;

void
boot(drive)
	int drive;
{
	int loadflags, currname = 0;
	char *t;
		

	/*
	gateA20(1);
	*/
	*((short *)0xb7002) = 0x44bb;
loadstart:
	/***************************************************************\
	* As a default set it to the first partition of the first	*
	* floppy or hard drive						*
	\***************************************************************/
#ifdef DOSREAD
	if (drive== 0xff) {
          maj = 5;
          part = 0;
          unit = 0;
	} else
#endif
        {
          part = 0;
          unit = drive&0x7f;
          maj = (drive&0x80 ? 0 : 2);         /* a good first bet */
        }

	name = names[currname++];

	loadflags = 0;
	switch(openrd()) {
	case 0:
		loadprog(loadflags);
		break;
	case -1:
		currname--;
		break;
	default:
		printf("Can't find %s\n", name);
		break;
	}
	if (currname == NUMNAMES)
		currname = 0;
	goto loadstart;
}

static void
loadprog(howto)
	int howto;
{
	u_long startaddr;
	u_long addr;	/* physical address.. not directly useable */
	int i;
	static int (*x_entry)() = 0;

	printf("loading %s...\n", name);

	read(&head, sizeof(head));
	if (N_BADMAG(head)) {
		printf("invalid format\n");
		return;
	}

	addr = startaddr = (int)head.a_entry;
#ifdef	DEBUG
	printf("Booting %s(%d,%c)%s @ 0x%x\n",
	    devs[maj], unit, 'a'+part, name, addr);
#endif

	/*
	 * The +40960 is for memory used by locore.s for the kernel page
	 * table and proc0 stack.  XXX
	 */
	if ((addr + N_BSSADDR(head) + head.a_bss + 40960) >
	    ((memsize(1) + 1024) * 1024)) {
		printf("kernel too large\n");
		return;
	}

	/********************************************************/
	/* LOAD THE TEXT SEGMENT				*/
	/********************************************************/
#ifdef	DEBUG
	printf("%d", head.a_text);
#endif
	xread(addr, head.a_text);
#ifdef CHECKSUM
	if (cflag)
		printf("(%x)", cksum(addr, head.a_text));
#endif
	addr += head.a_text;

	/********************************************************/
	/* Load the Initialised data after the text		*/
	/********************************************************/
	if (N_GETMAGIC(head) == NMAGIC) {
		i = CLBYTES - (addr & CLOFSET);
		if (i < CLBYTES) {
			pbzero(addr, i);
			addr += i;
		}
	}

#ifdef	DEBUG
	printf("+%d", head.a_data);
#endif
	xread(addr, head.a_data);
#ifdef CHECKSUM
	if (cflag)
		printf("(%x)", cksum(addr, head.a_data));
#endif
	addr += head.a_data;

	/********************************************************/
	/* Skip over the uninitialised data			*/
	/* (but clear it)					*/
	/********************************************************/
#ifdef	DEBUG
	printf("+%d", head.a_bss);
#endif
	pbzero(addr, head.a_bss);

	argv[3] = (addr += head.a_bss);

	/********************************************************/
	/* copy in the symbol header				*/
	/********************************************************/
	pcpy(&head.a_syms, addr, sizeof(head.a_syms));
	addr += sizeof(head.a_syms);

	if (head.a_syms == 0)
		goto nosyms;
	
	/********************************************************/
	/* READ in the symbol table				*/
	/********************************************************/
#ifdef	DEBUG
	printf("+[%d", head.a_syms);
#endif
	xread(addr, head.a_syms);
#ifdef CHECKSUM
	if (cflag)
		printf("(%x)", cksum(addr, head.a_syms));
#endif
	addr += head.a_syms;
	
	/********************************************************/
	/* Followed by the next integer (another header)	*/
	/* more debug symbols?					*/
	/********************************************************/
	read(&i, sizeof(int));
	pcpy(&i, addr, sizeof(int));
	if (i) {
		i -= sizeof(int);
		addr += sizeof(int);
#ifdef	DEBUG
		printf("+%d", i);
#endif
		xread(addr, i);
#ifdef CHECKSUM
		if (cflag)
			printf("(%x)", cksum(addr, i));
#endif
		addr += i;
	}

#ifdef	DEBUG
	putchar(']');
#endif
#ifdef DOSREAD
	doclose();
#endif

	/********************************************************/
	/* and that many bytes of (debug symbols?)		*/
	/********************************************************/
nosyms:
	argv[4] = ((addr+sizeof(int)-1))&~(sizeof(int)-1);

	/********************************************************/
	/* and note the end address of all this			*/
	/********************************************************/
#ifdef	DEBUG
	printf("=0x%x\n", addr);
#endif

#ifdef CHECKSUM
	if (cflag)
		return;
#endif

	/*
	 *  We now pass the various bootstrap parameters to the loaded
	 *  image via the argument list
	 *
         *  arg0 = 8 (magic)
	 *  arg1 = boot flags
	 *  arg2 = boot device
	 *  arg3 = start of symbol table (0 if not loaded)
	 *  arg4 = end of symbol table (0 if not loaded)
	 *  arg5 = transfer address from image
	 *  arg6 = transfer address for next image pointer
         *  arg7 = conventional memory size (640)
         *  arg8 = extended memory size (8196)
	 */
	/* startaddr &= 0xffffff; */
	argv[0] = 8;
	argv[1] = howto;
	argv[2] = (MAKEBOOTDEV(maj, 0, 0, unit, part));
	argv[5] = startaddr;
	argv[6] = (int) &x_entry;
	argv[7] = memsize(0);
	argv[8] = memsize(1);

	/****************************************************************/
	/* copy that first page and overwrite any BIOS variables	*/
	/****************************************************************/
#ifdef	DEBUG
	printf("entry point at 0x%x\n", (int)startaddr);
#endif
	startprog((int)startaddr, argv);
}
