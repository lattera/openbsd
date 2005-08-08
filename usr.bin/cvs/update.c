/*	$OpenBSD: src/usr.bin/cvs/update.c,v 1.44 2005/08/08 11:37:41 xsa Exp $	*/
/*
 * Copyright (c) 2004 Jean-Francois Brousseau <jfb@openbsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cvs.h"
#include "log.h"
#include "proto.h"


static int	cvs_update_init(struct cvs_cmd *, int, char **, int *);
static int	cvs_update_pre_exec(struct cvsroot *);
static int	cvs_update_remote(CVSFILE *, void *);
static int	cvs_update_local(CVSFILE *, void *);


struct cvs_cmd cvs_cmd_update = {
	CVS_OP_UPDATE, CVS_REQ_UPDATE, "update",
	{ "up", "upd" },
	"Bring work tree in sync with repository",
	"[-ACdflPpR] [-D date | -r rev] [-I ign] [-j rev] [-k mode] "
	"[-t id] ...",
	"ACD:dfI:j:k:lPpQqRr:t:",
	NULL,
	CF_SORT | CF_RECURSE | CF_IGNORE | CF_NOSYMS,
	cvs_update_init,
	cvs_update_pre_exec,
	cvs_update_remote,
	cvs_update_local,
	NULL,
	NULL,
	CVS_CMD_ALLOWSPEC | CVS_CMD_SENDARGS2 | CVS_CMD_SENDDIR
};

static char *date, *rev, *koptstr;
static int dflag, Aflag;
static int kflag = RCS_KWEXP_DEFAULT;

static int
cvs_update_init(struct cvs_cmd *cmd, int argc, char **argv, int *arg)
{
	int ch;

	dflag = Aflag = 0;
	date = NULL;
	rev = NULL;

	while ((ch = getopt(argc, argv, cmd->cmd_opts)) != -1) {
		switch (ch) {
		case 'A':
			Aflag = 1;
			break;
		case 'C':
		case 'D':
			date = optarg;
			break;
		case 'd':
			dflag = 1;
			break;
		case 'f':
			break;
		case 'I':
			break;
		case 'j':
			break;
		case 'k':
			koptstr = optarg;
			kflag = rcs_kflag_get(koptstr);
			if (RCS_KWEXP_INVAL(kflag)) {
				cvs_log(LP_ERR,
				    "invalid RCS keyword expansion mode");
				rcs_kflag_usage();
				return (CVS_EX_USAGE);
			}
			break;
		case 'l':
			cmd->file_flags &= ~CF_RECURSE;
			break;
		case 'P':
			cmd->cmd_flags |= CVS_CMD_PRUNEDIRS;
			break;
		case 'p':
			cvs_noexec = 1;	/* no locks will be created */
			break;
		case 'Q':
		case 'q':
			break;
		case 'R':
			cmd->file_flags |= CF_RECURSE;
			break;
		case 'r':
			rev = optarg;
			break;
		default:
			return (CVS_EX_USAGE);
		}
	}

	*arg = optind;
	return (0);
}

static int
cvs_update_pre_exec(struct cvsroot *root)
{
	if (root->cr_method != CVS_METHOD_LOCAL) {
		if ((cvs_cmd_update.cmd_flags & CVS_CMD_PRUNEDIRS) &&
		    (cvs_sendarg(root, "-P", 0) < 0))
			return (CVS_EX_PROTO);
		if (Aflag && cvs_sendarg(root, "-A", 0) < 0)
			return (CVS_EX_PROTO);
		if (dflag && cvs_sendarg(root, "-d", 0) < 0)
			return (CVS_EX_PROTO);

		if ((rev != NULL) && ((cvs_sendarg(root, "-r", 0) < 0) ||
		    (cvs_sendarg(root, rev, 0) < 0)))
			return (CVS_EX_PROTO);

		if ((date != NULL) && ((cvs_sendarg(root, "-D", 0) < 0) ||
		    (cvs_sendarg(root, date, 0) < 0)))
			return (CVS_EX_PROTO);
	}

	return (0);
}

/*
 * cvs_update_remote()
 *
 * Update a single file.  In the case where we act as client, send any
 * pertinent information about that file to the server.
 */
static int
cvs_update_remote(CVSFILE *cf, void *arg)
{
	int ret;
	char fpath[MAXPATHLEN];
	struct cvsroot *root;

	ret = 0;
	root = CVS_DIR_ROOT(cf);

	if (cf->cf_type == DT_DIR) {
		if (cf->cf_cvstat == CVS_FST_UNKNOWN)
			ret = cvs_sendreq(root, CVS_REQ_QUESTIONABLE,
			    cf->cf_name);
		else
			ret = cvs_senddir(root, cf);

		if (ret == -1)
			ret = CVS_EX_PROTO;

		return (ret);
	}

	cvs_file_getpath(cf, fpath, sizeof(fpath));

	if (cvs_sendentry(root, cf) < 0)
		return (CVS_EX_PROTO);

	if (!(cf->cf_flags & CVS_FILE_ONDISK))
		return (0);

	switch (cf->cf_cvstat) {
	case CVS_FST_UNKNOWN:
		ret = cvs_sendreq(root, CVS_REQ_QUESTIONABLE, cf->cf_name);
		break;
	case CVS_FST_UPTODATE:
		ret = cvs_sendreq(root, CVS_REQ_UNCHANGED, cf->cf_name);
		break;
	case CVS_FST_ADDED:
	case CVS_FST_MODIFIED:
		ret = cvs_sendreq(root, CVS_REQ_MODIFIED, cf->cf_name);
		if (ret == 0)
			ret = cvs_sendfile(root, fpath);
		break;
	default:
		break;
	}

	if (ret == -1)
		ret = CVS_EX_PROTO;

	return (ret);
}

/*
 * cvs_update_local()
 */
static int
cvs_update_local(CVSFILE *cf, void *arg)
{
	int ret;
	char fpath[MAXPATHLEN], rcspath[MAXPATHLEN];
	RCSFILE *rf;

	ret = 0;
	rf = NULL;

	cvs_file_getpath(cf, fpath, sizeof(fpath));

	if (cf->cf_cvstat == CVS_FST_UNKNOWN) {
		cvs_printf("? %s\n", fpath);
		return (CVS_EX_OK);
	}

	if (cf->cf_type == DT_DIR) {
		cvs_log(LP_NOTICE, "Updating %s", fpath);
		return (CVS_EX_OK);
	}

	if (cvs_rcs_getpath(cf, rcspath, sizeof(rcspath)) == NULL)
		return (CVS_EX_DATA);

	rf = rcs_open(rcspath, RCS_RDWR);
	if (rf == NULL) {
		printf("failed here?\n");
		return (CVS_EX_DATA);
	}

	rcs_close(rf);

	return (ret);
}
