/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/netif.h,v 1.2.16.1 2001/10/31 03:08:00 nate Exp $	*/


#include "iodesc.h"

struct netif {
	void *nif_devdata;
};

ssize_t		netif_get __P((struct iodesc *, void *, size_t, time_t));
ssize_t		netif_put __P((struct iodesc *, void *, size_t));

int		netif_open __P((void *));
int		netif_close __P((int));

struct iodesc	*socktodesc __P((int));

