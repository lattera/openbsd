/*	$OpenBSD: src/sys/arch/mvme68k/stand/bootst/dev_tape.h,v 1.1.16.2 2002/03/28 10:34:06 niklas Exp $	*/


int	tape_open(struct open_file *, ...);
int	tape_close(struct open_file *);
int	tape_strategy(void *, int, daddr_t, size_t, void *, size_t *);
int	tape_ioctl();

