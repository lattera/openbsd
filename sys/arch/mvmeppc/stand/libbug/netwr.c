/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/netwr.c,v 1.1.2.2 2004/02/19 10:49:10 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>

#include "libbug.h"

/* returns 0: success, nonzero: error */
int
mvmeprom_netwr(arg)
	struct mvmeprom_netio *arg;
{
	asm volatile ("mr 3, %0": : "r" (arg));
	MVMEPROM_CALL(MVMEPROM_NETWR);
	return (arg->status);
}
