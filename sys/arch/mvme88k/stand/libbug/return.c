/*	$OpenBSD: src/sys/arch/mvme88k/stand/libbug/return.c,v 1.1.8.1 2001/10/31 03:01:20 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>
#include "stand.h"
#include "libbug.h"

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
