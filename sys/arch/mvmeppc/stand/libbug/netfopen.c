/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/netfopen.c,v 1.1.2.2 2004/02/19 10:49:10 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>

#include "libbug.h"

/* returns 0: success, nonzero: error */
int
mvmeprom_netfopen(arg)
	struct mvmeprom_netfopen *arg;
{
	asm volatile ("mr 3, %0": : "r" (arg));
	MVMEPROM_CALL(MVMEPROM_NETFOPEN);
	return (arg->status);
}
