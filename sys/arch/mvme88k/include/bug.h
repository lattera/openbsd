/*	$OpenBSD: src/sys/arch/mvme88k/include/bug.h,v 1.4.6.3 2001/12/05 00:39:12 niklas Exp $ */

#ifndef _MACHINE_BUG_H_
#define _MACHINE_BUG_H_

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

#endif	/* _MACHINE_BUG_H_ */
