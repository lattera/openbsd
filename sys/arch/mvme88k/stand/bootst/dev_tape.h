/*	$OpenBSD: src/sys/arch/mvme88k/stand/bootst/dev_tape.h,v 1.1.6.1 2002/03/28 10:36:03 niklas Exp $ */

int	tape_open(struct open_file *, ...);
int	tape_close(struct open_file *);
int	tape_strategy(void *, int, daddr_t, size_t, void *, size_t *);
int	tape_ioctl();

