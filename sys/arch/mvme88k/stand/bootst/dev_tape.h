/*	$OpenBSD: src/sys/arch/mvme88k/stand/bootst/dev_tape.h,v 1.1.16.1 2002/06/11 03:37:11 art Exp $ */

int	tape_open(struct open_file *, ...);
int	tape_close(struct open_file *);
int	tape_strategy(void *, int, daddr_t, size_t, void *, size_t *);
int	tape_ioctl();

