/*	$OpenBSD: src/sys/arch/mvme88k/stand/netboot/dev_net.h,v 1.2.8.1 2002/03/28 10:36:03 niklas Exp $ */

int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

