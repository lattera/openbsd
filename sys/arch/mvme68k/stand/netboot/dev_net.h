/*	$OpenBSD: src/sys/arch/mvme68k/stand/netboot/dev_net.h,v 1.2.26.1 2002/06/11 03:36:51 art Exp $ */

int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

