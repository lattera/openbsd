/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/outln.c,v 1.1.2.2 2004/02/19 10:49:10 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>

#include "libbug.h"

void
mvmeprom_outln(start, end)
	char *start, *end;
{
	asm volatile ("mr 3, %0": : "r" (start));
	asm volatile ("mr 4, %0": : "r" (end));
	MVMEPROM_CALL(MVMEPROM_OUTLN);
}
