/*	$OpenBSD: src/sys/arch/mvme88k/include/bug.h,v 1.4.6.2 2001/10/31 03:01:18 nate Exp $ */
#ifndef __MACHINE_BUG_H__
#define __MACHINE_BUG_H__
#include <machine/bugio.h>

struct bugenv {
	int	clun;
	int	dlun;
	int	ipl;
	int	ctlr;
	int	(*entry) __P((void));
	int	cfgblk;
	char	*argstart;
	char	*argend;
};
#endif /* __MACHINE_BUG_H__ */
