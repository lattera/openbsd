/*	$OpenBSD: src/sys/arch/mvme68k/stand/bootxx/conf.c,v 1.1.16.1 2001/10/31 03:01:17 nate Exp $	*/
/*	$NetBSD: conf.c,v 1.2 1995/10/13 21:45:00 gwr Exp $	*/

#include <sys/types.h>
#include <machine/prom.h>

#include "stand.h"
#include "libsa.h"

struct devsw devsw[] = {
	{ "bugsc", bugscstrategy, bugscopen, bugscclose, bugscioctl },
};
int     ndevs = (sizeof(devsw)/sizeof(devsw[0]));

int debug;
