/*	$OpenBSD: src/sys/arch/mvme88k/stand/libbug/Attic/outch.c,v 1.1.8.1 2001/10/31 03:01:20 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>

void
mvmeprom_outchr(a)
	char a;
{
	asm volatile ("or r2, r0, %0" :  :"r" (a));
	BUG_CALL(_OUTCHR);
}

