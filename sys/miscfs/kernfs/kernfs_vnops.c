/*	$OpenBSD: src/sys/miscfs/kernfs/Attic/kernfs_vnops.c,v 1.4.4.8 1997/05/29 00:52:40 mickey Exp $	*/

/*
 * Copyright (c) 1996, 1997 Michael Shalayeff
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software donated to Berkeley by
 * Jan-Simon Pendry.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)kernfs_vnops.c	8.9 (Berkeley) 6/15/94
 */

/*
 * Kernel parameter filesystem (/kern)
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/vmmeter.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/malloc.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/namei.h>
#include <sys/buf.h>
#include <sys/dirent.h>
#include <sys/msgbuf.h>
#include <sys/exec.h>
#include <miscfs/kernfs/kernfs.h>
#include <vm/vm_param.h>
#include <machine/db_machdep.h>
#include <ddb/db_sym.h>

#define KSTRING	256		/* Largest I/O available via this filesystem */
#define	UIO_MX 32

#define	READ_MODE	(S_IRUSR|S_IRGRP|S_IROTH)
#define	WRITE_MODE	(S_IWUSR|READ_MODE)
#define DIR_MODE	(S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)

static int	byteorder = BYTE_ORDER;
static int	posix = _POSIX_VERSION;
static int	osrev = BSD;
static int	ncpu = 1;	/* XXX */
extern char machine[], cpu_model[];
extern char ostype[], osrelease[];

kern_target_t kern_targets[] = {
/* NOTE: The name must be less than UIO_MX-16 chars in length */
#define N(s) sizeof(s)-1, s
 /* type    name            data          tag          ktype  vtype mode */
  { DT_DIR, N("."),         0,            KTT_NULL,    Kroot, VDIR, DIR_MODE  },
  { DT_DIR, N(".."),        0,            KTT_NULL,    Kroot, VDIR, DIR_MODE  },
  { DT_REG, N("boottime"),  &boottime.tv_sec, KTT_INT, Kvar,  VREG, READ_MODE },
  { DT_REG, N("byteorder"), &byteorder,   KTT_INT,     Kvar,  VREG, READ_MODE },
  { DT_REG, N("copyright"), copyright,    KTT_STRING,  Kvar,  VREG, READ_MODE },
  { DT_REG, N("domainname"),0,            KTT_DOMAIN,  Kvar,  VREG, WRITE_MODE},
  { DT_REG, N("hostname"),  0,            KTT_HOSTNAME,Kvar,  VREG, WRITE_MODE},
  { DT_REG, N("hz"),        &hz,          KTT_INT,     Kvar,  VREG, READ_MODE },
  { DT_REG, N("loadavg"),   0,            KTT_AVENRUN, Kvar,  VREG, READ_MODE },
  { DT_REG, N("machine"),   machine,      KTT_STRING,  Kvar,  VREG, READ_MODE },
  { DT_REG, N("model"),     cpu_model,    KTT_STRING,  Kvar,  VREG, READ_MODE },
  { DT_REG, N("msgbuf"),    0,	          KTT_MSGBUF,  Kvar,  VREG, READ_MODE },
  { DT_REG, N("ncpu"),      &ncpu,        KTT_INT,     Kvar,  VREG, READ_MODE },
  { DT_REG, N("osrelease"), &osrelease,   KTT_STRING,  Kvar,  VREG, READ_MODE },
  { DT_REG, N("osrev"),     &osrev,       KTT_INT,     Kvar,  VREG, READ_MODE },
  { DT_REG, N("ostype"),    &ostype,      KTT_STRING,  Kvar,  VREG, READ_MODE },
  { DT_REG, N("pagesize"),  &cnt.v_page_size, KTT_INT, Kvar,  VREG, READ_MODE },
  { DT_REG, N("physmem"),   &physmem,     KTT_INT,     Kvar,  VREG, READ_MODE },
  { DT_REG, N("posix"),     &posix,       KTT_INT,     Kvar,  VREG, READ_MODE },
#if notdef
  { DT_DIR, N("root"),      0,            KTT_NULL,    Kvar,  VDIR, DIR_MODE  },
#endif
  { DT_BLK, N("rootdev"),   &rootdev,     KTT_DEVICE,  Kvar,  VBLK, READ_MODE },
  { DT_CHR, N("rrootdev"),  &rrootdev,    KTT_DEVICE,  Kvar,  VCHR, READ_MODE },
  { DT_DIR, N("sym"),       0,            KTT_NULL,    Ksym,  VDIR, DIR_MODE  },
  { DT_REG, N("time"),      0,            KTT_TIME,    Kvar,  VREG, READ_MODE },
  { DT_REG, N("usermem"),   0,            KTT_USERMEM, Kvar,  VREG, READ_MODE },
  { DT_REG, N("version"),   version,      KTT_STRING,  Kvar,  VREG, READ_MODE },
#if notdef
  { DT_DIR, N("var"),       0,            KTT_NULL,    Kvar,  VDIR, DIR_MODE  },
#endif
#undef N
  { 0,      0, NULL,        0,            KTT_NULL,    0,     0,    0         }
};
#define	kern_ntargets	(sizeof(kern_targets)/sizeof(kern_targets[0]))

int	kernfs_badop	__P((void *));
int	kernfs_enotsupp __P((void *));

int	kernfs_lookup	__P((void *));
#define	kernfs_create	kernfs_enotsupp
#define	kernfs_mknod	kernfs_enotsupp
int	kernfs_open	__P((void *));
int	kernfs_close	__P((void *));
int	kernfs_access	__P((void *));
int	kernfs_getattr	__P((void *));
int	kernfs_setattr	__P((void *));
int	kernfs_read	__P((void *));
int	kernfs_write	__P((void *));
#define	kernfs_ioctl	kernfs_enotsupp
#define	kernfs_select	kernfs_enotsupp
#define	kernfs_mmap	kernfs_enotsupp
#define	kernfs_fsync	nullop
#define	kernfs_seek	nullop
#define	kernfs_remove	kernfs_enotsupp
int	kernfs_link	__P((void *));
#define	kernfs_rename	kernfs_enotsupp
#define	kernfs_mkdir	kernfs_enotsupp
#define	kernfs_rmdir	kernfs_enotsupp
int	kernfs_symlink	__P((void *));
int	kernfs_readdir	__P((void *));
#define	kernfs_readlink	kernfs_enotsupp
int	kernfs_abortop	__P((void *));
int	kernfs_inactive	__P((void *));
int	kernfs_reclaim	__P((void *));
#define	kernfs_lock	nullop
#define	kernfs_unlock	nullop
#define	kernfs_bmap	kernfs_badop
#define	kernfs_strategy	kernfs_badop
int	kernfs_print	__P((void *));
#define	kernfs_islocked	nullop
int	kernfs_pathconf	__P((void *));
#define	kernfs_advlock	kernfs_enotsupp
#define	kernfs_blkatoff	kernfs_enotsupp
#define	kernfs_valloc	kernfs_enotsupp
int	kernfs_vfree	__P((void *));
#define	kernfs_truncate	kernfs_enotsupp
#define	kernfs_update	kernfs_enotsupp
#define	kernfs_bwrite	kernfs_enotsupp

int	kernfs_xread __P((kern_target_t *, int, char **, int));
int	kernfs_xwrite __P((kern_target_t *, char *, int));

int (**kernfs_vnodeop_p) __P((void *));
struct vnodeopv_entry_desc kernfs_vnodeop_entries[] = {
	{ &vop_default_desc, vn_default_error },
	{ &vop_lookup_desc, kernfs_lookup },	/* lookup */
	{ &vop_create_desc, kernfs_create },	/* create */
	{ &vop_mknod_desc, kernfs_mknod },	/* mknod */
	{ &vop_open_desc, kernfs_open },	/* open */
	{ &vop_close_desc, kernfs_close },	/* close */
	{ &vop_access_desc, kernfs_access },	/* access */
	{ &vop_getattr_desc, kernfs_getattr },	/* getattr */
	{ &vop_setattr_desc, kernfs_setattr },	/* setattr */
	{ &vop_read_desc, kernfs_read },	/* read */
	{ &vop_write_desc, kernfs_write },	/* write */
	{ &vop_ioctl_desc, kernfs_ioctl },	/* ioctl */
	{ &vop_select_desc, kernfs_select },	/* select */
	{ &vop_mmap_desc, kernfs_mmap },	/* mmap */
	{ &vop_fsync_desc, kernfs_fsync },	/* fsync */
	{ &vop_seek_desc, kernfs_seek },	/* seek */
	{ &vop_remove_desc, kernfs_remove },	/* remove */
	{ &vop_link_desc, kernfs_link },	/* link */
	{ &vop_rename_desc, kernfs_rename },	/* rename */
	{ &vop_mkdir_desc, kernfs_mkdir },	/* mkdir */
	{ &vop_rmdir_desc, kernfs_rmdir },	/* rmdir */
	{ &vop_symlink_desc, kernfs_symlink },	/* symlink */
	{ &vop_readdir_desc, kernfs_readdir },	/* readdir */
	{ &vop_readlink_desc, kernfs_readlink },/* readlink */
	{ &vop_abortop_desc, kernfs_abortop },	/* abortop */
	{ &vop_inactive_desc, kernfs_inactive },/* inactive */
	{ &vop_reclaim_desc, kernfs_reclaim },	/* reclaim */
	{ &vop_lock_desc, kernfs_lock },	/* lock */
	{ &vop_unlock_desc, kernfs_unlock },	/* unlock */
	{ &vop_bmap_desc, kernfs_bmap },	/* bmap */
	{ &vop_strategy_desc, kernfs_strategy },/* strategy */
	{ &vop_print_desc, kernfs_print },	/* print */
	{ &vop_islocked_desc, kernfs_islocked },/* islocked */
	{ &vop_pathconf_desc, kernfs_pathconf },/* pathconf */
	{ &vop_advlock_desc, kernfs_advlock },	/* advlock */
	{ &vop_blkatoff_desc, kernfs_blkatoff },/* blkatoff */
	{ &vop_valloc_desc, kernfs_valloc },	/* valloc */
	{ &vop_vfree_desc, kernfs_vfree },	/* vfree */
	{ &vop_truncate_desc, kernfs_truncate },/* truncate */
	{ &vop_update_desc, kernfs_update },	/* update */
	{ &vop_bwrite_desc, kernfs_bwrite },	/* bwrite */
	{ (struct vnodeop_desc*)NULL, (int(*) __P((void *)))NULL }
};
struct vnodeopv_desc kernfs_vnodeop_opv_desc =
	{ &kernfs_vnodeop_p, kernfs_vnodeop_entries };

int
kernfs_xread(kt, off, bufp, len)
	kern_target_t *kt;
	int off;
	char **bufp;
	int len;
{

	switch (kt->kt_tag) {
	case KTT_TIME: {
		struct timeval tv;

		microtime(&tv);
		sprintf(*bufp, "%ld %ld\n", tv.tv_sec, tv.tv_usec);
		break;
	}

	case KTT_INT: {
		int *ip = kt->kt_data;

		sprintf(*bufp, "%d\n", *ip);
		break;
	}

	case KTT_STRING: {
		char *cp = kt->kt_data;

		*bufp = cp;
		break;
	}

	case KTT_MSGBUF: {
		extern struct msgbuf *msgbufp;
		long n;

		if (off >= MSG_BSIZE)
			return (0);
		n = msgbufp->msg_bufx + off;
		if (n >= MSG_BSIZE)
			n -= MSG_BSIZE;
		len = min(MSG_BSIZE - n, MSG_BSIZE - off);
		*bufp = msgbufp->msg_bufc + n;
		return (len);
	}

	case KTT_HOSTNAME: {
		char *cp = hostname;
		int xlen = hostnamelen;

		if (xlen >= (len-2))
			return (EINVAL);

		bcopy(cp, *bufp, xlen);
		(*bufp)[xlen] = '\n';
		(*bufp)[xlen+1] = '\0';
		break;
	}

	case KTT_DOMAIN: {
		char *cp = domainname;
		int xlen = domainnamelen;

		if (xlen >= (len-2))
			return (EINVAL);

		bcopy(cp, *bufp, xlen);
		(*bufp)[xlen] = '\n';
		(*bufp)[xlen+1] = '\0';
		break;
	}

	case KTT_AVENRUN:
		averunnable.fscale = FSCALE;
		sprintf(*bufp, "%d %d %d %ld\n",
		    averunnable.ldavg[0], averunnable.ldavg[1],
		    averunnable.ldavg[2], averunnable.fscale);
		break;

	case KTT_USERMEM:
		sprintf(*bufp, "%lu\n", physmem - cnt.v_wire_count);
		break;

	default:
		return (0);
	}

	len = strlen(*bufp);
	if (len <= off)
		return (0);
	*bufp += off;
	return (len - off);
}

int
kernfs_xwrite(kt, buf, len)
	kern_target_t *kt;
	char *buf;
	int len;
{

	switch (kt->kt_tag) {
	case KTT_DOMAIN:
		if (buf[len-1] == '\n')
			--len;
		bcopy(buf, domainname, len);
		domainname[len] = '\0';
		domainnamelen = len;
		return (0);

	case KTT_HOSTNAME:
		if (buf[len-1] == '\n')
			--len;
		bcopy(buf, hostname, len);
		hostname[len] = '\0';
		hostnamelen = len;
		return (0);

	default:
		return (EIO);
	}
}


/*
 * vp is the current namei directory
 * ndp is the name to locate in that directory...
 */
int
kernfs_lookup(v)
	void *v;
{
	struct vop_lookup_args /* {
		struct vnode * a_dvp;
		struct vnode ** a_vpp;
		struct componentname * a_cnp;
	} */ *ap = v;
	struct componentname *cnp = ap->a_cnp;
	struct vnode **vpp = ap->a_vpp;
	struct vnode *dvp = ap->a_dvp;
	char *pname = cnp->cn_nameptr;
	struct vnode *fvp;
	struct kernfs_node *kfs;
	kern_target_t *kt;
	int error;

#ifdef KERNFS_DIAGNOSTIC
	printf("kernfs_lookup(%x)\n", ap);
	printf("kernfs_lookup(dp = %x, vpp = %x, cnp = %x)\n", dvp, vpp, ap->a_cnp);
	printf("kernfs_lookup(%s)\n", pname);
#endif

	*vpp = NULLVP;

	if (cnp->cn_nameiop == DELETE || cnp->cn_nameiop == RENAME)
		return (EROFS);

	if (cnp->cn_namelen == 1 && *pname == '.') {
		*vpp = dvp;
		VREF(dvp);
		/*VOP_LOCK(dvp);*/
		return (0);
	}

	kfs = VTOKERN(dvp);
	switch (kfs->kf_type) {
	case Kroot:
	{
		if (cnp->cn_flags & ISDOTDOT)
			return (EIO);
#if 0
		if (cnp->cn_namelen == 4 && bcmp(pname, "root", 4) == 0) {
			*vpp = rootdir;
			VREF(rootdir);
			VOP_LOCK(rootdir);
			return (0);
		}
#endif

		for (kt = kern_targets; kt->kt_name != NULL; kt++) {
			if (cnp->cn_namelen == kt->kt_namlen &&
			    bcmp(kt->kt_name, pname, cnp->cn_namelen) == 0)
				break;
		}

		if (kt->kt_name == NULL) {
#ifdef KERNFS_DIAGNOSTIC
			printf("kernfs_lookup: pname = %s, failed", pname);
#endif

			return (cnp->cn_nameiop == LOOKUP ? ENOENT : EROFS);
		}

		if (kt->kt_tag == KTT_DEVICE) {
			dev_t *dp = kt->kt_data;

			do {
				if (*dp == NODEV || !vfinddev(*dp, kt->kt_vtype, &fvp))
					return (ENOENT);
				*vpp = fvp;
			} while (vget(fvp, 1));

			return (0);
		}

#ifdef KERNFS_DIAGNOSTIC
		printf("kernfs_lookup: allocate new vnode\n");
#endif
		if ((error = kernfs_allocvp(dvp->v_mount, vpp, kt, kt->kt_ktype)) != 0)
			return error;

#ifdef KERNFS_DIAGNOSTIC
		printf("kernfs_lookup: newvp = %p\n", fvp);
#endif
		return 0;
	}
	case Ksym:
	{
		db_symtab_t	st;

		if (cnp->cn_flags & ISDOTDOT)
			return (kernfs_root(dvp->v_mount, vpp));

		for (st = db_symiter(NULL); st != NULL; st = db_symiter(st))
			if (cnp->cn_namelen == strlen(st->name) &&
			     bcmp(st->name, pname, cnp->cn_namelen) == 0)
				break;

		if (st == NULL) {                               
#ifdef KERNFS_DIAGNOSTIC                                      
			printf("kernfs_lookup: pname = %s, failed", pname);
#endif
			return (cnp->cn_nameiop == LOOKUP ? ENOENT : EROFS);
		}
		
		if (!(error = kernfs_allocvp(dvp->v_mount, vpp, NULL, Ksymtab ))) {
			kfs = VTOKERN(*vpp);
			kfs->kf_st = st;
			db_stub_xh(st, &kfs->kf_xh);
		}
		return error;
	}
	default:
		return (ENOTDIR);
	}

	return (0);
}

int
kernfs_open(v)
	void *v;
{
	struct vop_open_args /* {
		struct vnode *a_vp;
		int  a_mode;
		struct ucred *a_cred;
		struct proc *a_p;
	} */ *ap = v;
	struct kernfs_node *kfs = VTOKERN(ap->a_vp);

	switch (kfs->kf_type) {
	case Ksymtab:
		if (((kfs->kf_flags & FWRITE) && (ap->a_mode & O_EXCL)) ||
		    ((kfs->kf_flags & O_EXCL) && (ap->a_mode & FWRITE)))
			return (EBUSY);

		if (ap->a_mode & FWRITE)
			kfs->kf_flags = ap->a_mode & (FWRITE|O_EXCL);
		break;
	default:
		break;
	}

	/* Only need to check access permissions. */
	return (0);
}

int
kernfs_close(v)
	void *v;
{
	struct vop_close_args /* {
		struct vnode *a_vp;
		int  a_fflag;
		struct ucred *a_cred;
		struct proc *a_p;
	} */ *ap = v;
	struct kernfs_node *kfs = VTOKERN(ap->a_vp);

	switch (kfs->kf_type) {
	case Ksymtab:
		if ((ap->a_fflag & FWRITE) && (kfs->kf_flags & O_EXCL))
			kfs->kf_flags &= ~(FWRITE|O_EXCL);
		break;
	default:
		break;
	}

	return 0;
}

int
kernfs_access(v)
	void *v;
{
	struct vop_access_args /* {
		struct vnode *a_vp;
		int a_mode;
		struct ucred *a_cred;
		struct proc *a_p;
	} */ *ap = v;
	struct vnode *vp = ap->a_vp;
	mode_t fmode =
	    (vp->v_flag & VROOT) ? DIR_MODE : VTOKERN(vp)->kf_mode;

	return (vaccess(fmode, (uid_t)0, (gid_t)0, ap->a_mode, ap->a_cred));
}

int
kernfs_getattr(v)
	void *v;
{
	struct vop_getattr_args /* {
		struct vnode *a_vp;
		struct vattr *a_vap;
		struct ucred *a_cred;
		struct proc *a_p;
	} */ *ap = v;
	struct vnode *vp = ap->a_vp;
	struct kernfs_node *kfs = VTOKERN(vp);
	kern_target_t *kt = kfs->kf_kt;
	struct vattr *vap = ap->a_vap;
	struct timeval tv;
	int error = 0;

	/* start by zeroing out the attributes */
	VATTR_NULL(vap);

	vap->va_uid = 0;
	vap->va_gid = 0;
	vap->va_fsid = vp->v_mount->mnt_stat.f_fsid.val[0];
	vap->va_blocksize = DEV_BSIZE;
	microtime(&tv);
	TIMEVAL_TO_TIMESPEC(&tv, &vap->va_atime);
	vap->va_mtime = vap->va_atime;
	vap->va_ctime = vap->va_atime;
	vap->va_flags = 0;
	vap->va_bytes = vap->va_size = 0;
	vap->va_mode = kfs->kf_mode;

	if (vp->v_flag & VROOT) {
#ifdef KERNFS_DIAGNOSTIC
		printf("kernfs_getattr: stat rootdir\n");
#endif
		vap->va_type = VDIR;
		vap->va_nlink = 2;
		vap->va_fileid = 2;
		vap->va_size = DEV_BSIZE;

	} else if (kfs->kf_type == Ksymtab) {

#ifdef KERNFS_DIAGNOSTIC
		printf("kernfs_getattr: stat symtab %s\n", kfs->kf_st->name);
#endif
		vap->va_type = VREG;
		vap->va_nlink = 1;
		vap->va_fileid = 3 + kern_ntargets + kfs->kf_st->id;
		vap->va_size = sizeof(struct exec) + kfs->kf_xh.a_text +
			kfs->kf_xh.a_data + db_symtablen(kfs->kf_st);
	} else {
#ifdef KERNFS_DIAGNOSTIC
		printf("kernfs_getattr: stat target %s\n", kt->kt_name);
#endif
		vap->va_type = kt->kt_vtype;
		vap->va_fileid = 3 + (kfs->kf_kt - kern_targets);
		if (kfs->kf_type == Ksym) {
			vap->va_nlink = 2;
			vap->va_size = DEV_BSIZE;
		} else {
			int nbytes = 0, total = 0;
			char strbuf[KSTRING], *p = strbuf;

			do {
				p = strbuf;
				total += nbytes;
			} while ((nbytes =
				kernfs_xread(kt, total, &p,
					     sizeof(strbuf)) != 0));
			vap->va_size = total;
			vap->va_nlink = 1;
		}
	}

#ifdef KERNFS_DIAGNOSTIC
	printf("kernfs_getattr: return error %d\n", error);
#endif
	return (error);
}

/*ARGSUSED*/
int
kernfs_setattr(v)
	void *v;
{
	/*
	 * Silently ignore attribute changes.
	 * This allows for open with truncate to have no
	 * effect until some data is written.  I want to
	 * do it this way because all writes are atomic.
	 */
	return (0);
}

int
kernfs_read(v)
	void *v;
{
	struct vop_read_args /* {
		struct vnode *a_vp;
		struct uio *a_uio;
		int  a_ioflag;
		struct ucred *a_cred;
	} */ *ap = v;
	struct vnode *vp = ap->a_vp;
	struct kernfs_node *kfs = VTOKERN(vp);
	struct uio *uio = ap->a_uio;
	kern_target_t *kt = kfs->kf_kt;
	char strbuf[KSTRING], *buf;
	int off, len;
	int error;

	if (vp->v_type == VDIR)
		return (EOPNOTSUPP);


	off = uio->uio_offset;
	switch (kfs->kf_type) {
	case Kvar:
#ifdef KERNFS_DIAGNOSTIC
		printf("kern_read %s\n", kt->kt_name);
#endif
#if 0
		while (buf = strbuf,
#else
		if (buf = strbuf,
#endif
		    len = kernfs_xread(kt, off, &buf, sizeof(strbuf))) {
			if ((error = uiomove(buf, len, uio)) != 0)
				return (error);
			off += len;
		}
		break;
	case Ksymtab:
	{
		caddr_t p;
		char buf[512];
#ifdef KERNFS_DIAGNOSTIC
		printf("kern_read %s, off = %d\n", kfs->kf_st->name, off);
#endif
		if (off < sizeof(struct exec)) {
			len = sizeof(struct exec);
			p = (caddr_t)&kfs->kf_xh;
		} else if (off < N_SYMOFF(kfs->kf_xh)) {
			return EIO;
		} else {
			off -= N_SYMOFF(kfs->kf_xh);
			len = sizeof(buf);
			p = buf;
			if (db_symatoff(kfs->kf_st, off, buf, &len))
				return EIO;
			off = 0;
		}
		len = min(len - off, uio->uio_resid);
		if ((error = uiomove(p + off, len, uio)) != 0)
			return error;
	}
		break;
	default:
		break;
	}

	return (0);
}

int
kernfs_write(v)
	void *v;
{
	struct vop_write_args /* {
		struct vnode *a_vp;
		struct uio *a_uio;
		int  a_ioflag;
		struct ucred *a_cred;
	} */ *ap = v;
	struct vnode *vp = ap->a_vp;
	struct uio *uio = ap->a_uio;
	kern_target_t *kt;
	int error, xlen;
	char strbuf[KSTRING];

	if (vp->v_type == VDIR)
		return (EOPNOTSUPP);

	kt = VTOKERN(vp)->kf_kt;

	if (uio->uio_offset != 0)
		return (EINVAL);

	xlen = min(uio->uio_resid, KSTRING-1);
	if ((error = uiomove(strbuf, xlen, uio)) != 0)
		return (error);

	if (uio->uio_resid != 0)
		return (EIO);

	strbuf[xlen] = '\0';
	xlen = strlen(strbuf);
	return (kernfs_xwrite(kt, strbuf, xlen));
}

int
kernfs_readdir(v)
	void *v;
{
	struct vop_readdir_args /* {
		struct vnode *a_vp;
		struct uio *a_uio;
		struct ucred *a_cred;
		int *a_eofflag;
		u_long *a_cookies;
		int a_ncookies;
	} */ *ap = v;
	struct uio *uio = ap->a_uio;
	struct dirent d;
	struct kernfs_node *kfs;
	int i;
	int error = 0;
	u_long *cookies = ap->a_cookies;
	int ncookies = ap->a_ncookies;

	if (ap->a_vp->v_type != VDIR)
		return (ENOTDIR);

	if (uio->uio_resid < UIO_MX)
		return (EINVAL);

	i = uio->uio_offset;
	bzero((caddr_t)&d, UIO_MX);
	d.d_reclen = UIO_MX;
	kfs = VTOKERN(ap->a_vp);

	switch (kfs->kf_type) {
	case Kroot:
	{
		register kern_target_t *kt;

		for (kt = &kern_targets[i];
		     uio->uio_resid >= UIO_MX &&
			 kt->kt_name != NULL &&
			 i < kern_ntargets;
		     kt++, i++) {
#ifdef KERNFS_DIAGNOSTIC
			printf("kernfs_readdir: kt = %s\n", kt->kt_name);
#endif

			if (kt->kt_tag == KTT_DEVICE) {
				dev_t *dp = kt->kt_data;
				struct vnode *fvp;

				if (*dp == NODEV || !vfinddev(*dp, kt->kt_vtype, &fvp))
					continue;
			}

			d.d_fileno = i + 3;
			d.d_namlen = kt->kt_namlen;
			bcopy(kt->kt_name, d.d_name, kt->kt_namlen + 1);
			d.d_type = kt->kt_type;

			if ((error = uiomove((caddr_t)&d, UIO_MX, uio)) != 0)
				break;
			if (ncookies-- > 0)
				*cookies++ = i + 1;
		}
	}
	break;
	case Ksym:
	{
		register db_symtab_t	st;

#ifdef KERNFS_DIAGNOSTIC
		printf("kernfs_readdir: i = %d\n", i);
#endif
		if (i == 0 || i == 1) {
			d.d_fileno = 3+ ((i==1)? -1: kfs->kf_kt - kern_targets);
			d.d_namlen = i + 1;
			bcopy("..", d.d_name, d.d_namlen);
			d.d_name[i + 1] = '\0';
			d.d_type = DT_DIR;

			if ((error = uiomove((caddr_t)&d, UIO_MX, uio)) != 0)
				break;
			if (ncookies-- > 0)
				*cookies++ = i + 1;
			i++;
		} else {
			register int j = i - 2;
			for (st = db_symiter(NULL);
			     st != NULL && j--;st = db_symiter(st));

			for (;st != NULL &&
			     uio->uio_resid >= UIO_MX; i++, st = db_symiter(st)) {
				d.d_fileno = st->id + 3 + kern_ntargets;
				d.d_namlen = strlen(st->name);
				bcopy(st->name, d.d_name, d.d_namlen+1);
				d.d_type = DT_REG;

				if ((error = uiomove((caddr_t)&d, UIO_MX, uio)) != 0)
					break;
				if (ncookies-- > 0)
					*cookies++ = i + 1;
			}
		}
	}
	break;
	default:
		error = ENOTDIR;
		break;
	}

	uio->uio_offset = i;
	return (error);
}

int
kernfs_inactive(v)
	void *v;
{
	struct vop_inactive_args /* {
		struct vnode *a_vp;
	} */ *ap = v;
	struct vnode *vp = ap->a_vp;
	struct kernfs_node *kfs = VTOKERN(vp);
	db_symtab_t	st;

#ifdef KERNFS_DIAGNOSTIC
	printf("kernfs_inactive(%x)\n", vp);
#endif

	if (kfs == NULL || kfs->kf_type != Ksymtab)
		return 0;

	for (st = db_symiter(NULL); st != NULL && st != kfs->kf_st;
	     st = db_symiter(st))
		;

	if (st == NULL)
		vgone(vp);
	return (0);
}

int
kernfs_reclaim(v)
	void *v;
{
	struct vop_reclaim_args /* {
		struct vnode *a_vp;
	} */ *ap = v;
#ifdef KERNFS_DIAGNOSTIC
	struct vnode *vp = ap->a_vp;

	printf("kernfs_reclaim(%x)\n", vp);
#endif
	return (kernfs_freevp(ap->a_vp));
}

/*
 * Return POSIX pathconf information applicable to special devices.
 */
int
kernfs_pathconf(v)
	void *v;
{
	struct vop_pathconf_args /* {
		struct vnode *a_vp;
		int a_name;
		register_t *a_retval;
	} */ *ap = v;

	switch (ap->a_name) {
	case _PC_LINK_MAX:
		*ap->a_retval = LINK_MAX;
		return (0);
	case _PC_MAX_CANON:
		*ap->a_retval = MAX_CANON;
		return (0);
	case _PC_MAX_INPUT:
		*ap->a_retval = MAX_INPUT;
		return (0);
	case _PC_PIPE_BUF:
		*ap->a_retval = PIPE_BUF;
		return (0);
	case _PC_CHOWN_RESTRICTED:
		*ap->a_retval = 1;
		return (0);
	case _PC_VDISABLE:
		*ap->a_retval = _POSIX_VDISABLE;
		return (0);
	default:
		return (EINVAL);
	}
	/* NOTREACHED */
}

/*
 * Print out the contents of a /dev/fd vnode.
 */
/* ARGSUSED */
int
kernfs_print(v)
	void *v;
{
	struct vop_print_args /* {
		struct vnode *a_vp;
	} */ *ap = v;
	struct kernfs_node *kfs = VTOKERN(ap->a_vp);

	printf("tag VT_KERNFS, type %d, mode %x, flags %lx\n",
		kfs->kf_type, kfs->kf_mode, kfs->kf_flags);
	return (0);
}

/*ARGSUSED*/
int
kernfs_vfree(v)
	void *v;
{

	return (0);
}

int
kernfs_link(v) 
	void *v;
{
	struct vop_link_args /* {
		struct vnode *a_dvp;
		struct vnode *a_vp;  
		struct componentname *a_cnp;
	} */ *ap = v;
 
	VOP_ABORTOP(ap->a_dvp, ap->a_cnp);
	vput(ap->a_dvp);
	return (EROFS);
}

int
kernfs_symlink(v)
	void *v;
{
	struct vop_symlink_args /* {
		struct vnode *a_dvp;
		struct vnode **a_vpp;
		struct componentname *a_cnp;
		struct vattr *a_vap;
		char *a_target;
	} */ *ap = v;
  
	VOP_ABORTOP(ap->a_dvp, ap->a_cnp);
	vput(ap->a_dvp);
	return (EROFS);
}

int
kernfs_abortop(v)
	void *v;
{
	struct vop_abortop_args /* {
		struct vnode *a_dvp;
		struct componentname *a_cnp;
	} */ *ap = v;
 
	if ((ap->a_cnp->cn_flags & (HASBUF | SAVESTART)) == HASBUF)
		FREE(ap->a_cnp->cn_pnbuf, M_NAMEI);
	return (0);
}

/*
 * /dev/fd vnode unsupported operation
 */
/*ARGSUSED*/
int
kernfs_enotsupp(v)
	void *v;
{

	return (EOPNOTSUPP);
}

/*
 * /dev/fd "should never get here" operation
 */
/*ARGSUSED*/
int
kernfs_badop(v)
	void *v;
{

	panic("kernfs: bad op");
	return 0;
}
