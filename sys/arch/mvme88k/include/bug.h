/*	$OpenBSD: src/sys/arch/mvme88k/include/bug.h,v 1.8.2.1 2002/06/11 03:37:10 art Exp $ */

#ifndef _MACHINE_BUG_H_
#define _MACHINE_BUG_H_

struct bugenv {
	int	clun;
	int	dlun;
	int	ipl;
	int	ctlr;
	int	(*entry)(void);
	int	cfgblk;
	char	*argstart;
	char	*argend;
};

#endif	/* _MACHINE_BUG_H_ */
