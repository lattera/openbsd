/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/outch.c,v 1.1.2.2 2004/02/19 10:49:10 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>

#include "libbug.h"

void
mvmeprom_outchr(a)
	char a;
{
	asm volatile ("mr 3, %0" :  :"r" (a));
	MVMEPROM_CALL(MVMEPROM_OUTCHR);
}

