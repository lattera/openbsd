/*	$OpenBSD: src/sys/arch/mvme88k/stand/libbug/return.c,v 1.1.8.2 2004/02/19 10:49:09 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>
#include "stand.h"
#include "prom.h"

/* BUG - return to bug routine */
void
mvmeprom_return()
{
	MVMEPROM_CALL(MVMEPROM_EXIT);
	/*NOTREACHED*/
}

/* BUG - return to bug routine */
__dead void
_rtt()
{
	MVMEPROM_CALL(MVMEPROM_EXIT);
	printf("_rtt: exit failed.  spinning...");
	while (1) ;
	/*NOTREACHED*/
}
