/*	$OpenBSD: src/sys/arch/mvme68k/stand/netboot/dev_net.h,v 1.2.16.1 2002/03/28 10:34:06 niklas Exp $ */

int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

