/*	$OpenBSD: src/sys/lib/libsa/bootparam.h,v 1.3.8.1 2002/03/28 15:02:00 niklas Exp $	*/

int bp_whoami(int sock);
int bp_getfile(int sock, char *key, struct in_addr *addrp, char *path);

