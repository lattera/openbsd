/*	$OpenBSD: src/sys/arch/mvme88k/stand/netboot/dev_net.h,v 1.2.18.1 2002/06/11 03:37:11 art Exp $ */

int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

