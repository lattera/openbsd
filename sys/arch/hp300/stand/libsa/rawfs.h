/*	$OpenBSD: src/sys/arch/hp300/stand/libsa/rawfs.h,v 1.1.12.1 2002/03/28 10:07:19 niklas Exp $	*/
/*	$NetBSD: rawfs.h,v 1.1 1996/06/26 17:44:36 thorpej Exp $	*/

/*
 * Raw file system - for stream devices like tapes.
 * No random access, only sequential read allowed.
 */

int	rawfs_open(char *path, struct open_file *f);
int	rawfs_close(struct open_file *f);
int	rawfs_read(struct open_file *f, void *buf,
		u_int size, u_int *resid);
int	rawfs_write(struct open_file *f, void *buf,
		u_int size, u_int *resid);
off_t	rawfs_seek(struct open_file *f, off_t offset, int where);
int	rawfs_stat(struct open_file *f, struct stat *sb);
