/*	$OpenBSD: src/sys/arch/sun3/stand/netboot/Attic/dev_net.h,v 1.1.1.1.16.1 2001/10/31 03:08:00 nate Exp $	*/


int	net_open __P((struct open_file *, ...));
int	net_close __P((struct open_file *));
int	net_ioctl();
int	net_strategy();

