/*	$OpenBSD: src/sys/arch/mvme88k/stand/netboot/dev_net.h,v 1.2 1998/08/22 08:37:57 smurph Exp $ */

int	net_open __P((struct open_file *, ...));
int	net_close __P((struct open_file *));
int	net_ioctl();
int	net_strategy();

