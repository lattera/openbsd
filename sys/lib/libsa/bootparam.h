/*	$OpenBSD: src/sys/lib/libsa/bootparam.h,v 1.3.18.1 2002/06/11 03:30:09 art Exp $	*/

int bp_whoami(int sock);
int bp_getfile(int sock, char *key, struct in_addr *addrp, char *path);

