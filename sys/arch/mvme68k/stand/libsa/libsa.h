/*	$OpenBSD: src/sys/arch/mvme68k/stand/libsa/libsa.h,v 1.3.14.1 2001/10/31 03:01:18 nate Exp $	*/

/*
 * libsa prototypes 
 */

#include "libbug.h"

/* bugdev.c */
int bugscopen __P((struct open_file *, ...));
int bugscclose __P((struct open_file *));
int bugscioctl __P((struct open_file *, u_long, void *));
int bugscstrategy __P((void *, int, daddr_t, size_t, void *, size_t *));

/* exec_mvme.c */
void exec_mvme __P((char *, int));

/* parse_args.c */
int parse_args __P((char **, int *));

