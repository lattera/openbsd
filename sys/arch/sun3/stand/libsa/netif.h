/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/netif.h,v 1.3.4.1 2002/06/11 03:39:01 art Exp $	*/


#include "iodesc.h"

struct netif {
	void *nif_devdata;
};

ssize_t		netif_get(struct iodesc *, void *, size_t, time_t);
ssize_t		netif_put(struct iodesc *, void *, size_t);

int		netif_open(void *);
int		netif_close(int);

struct iodesc	*socktodesc(int);

