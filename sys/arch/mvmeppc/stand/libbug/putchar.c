/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/putchar.c,v 1.1.2.1 2004/02/19 10:49:10 niklas Exp $ */

/*
 * putchar: easier to do this with outstr than to add more macros to
 * handle byte passing on the stack
 */

#include <sys/types.h>

#include "libbug.h"

void
putchar(c)
	int c;
{
	char ca;
	ca = (char)c & 0xFF;
	if (ca == '\n')
		putchar('\r');
	
	asm  volatile ("mr 3, %0" :: "r" (ca));
	MVMEPROM_CALL(MVMEPROM_OUTCHR);
}
