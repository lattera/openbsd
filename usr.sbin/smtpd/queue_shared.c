/*	$OpenBSD: src/usr.sbin/smtpd/Attic/queue_shared.c,v 1.41 2011/04/14 22:36:09 gilles Exp $	*/

/*
 * Copyright (c) 2008 Gilles Chehade <gilles@openbsd.org>
 * Copyright (c) 2008 Pierre-Yves Ritschard <pyr@openbsd.org>
 * Copyright (c) 2008-2009 Jacek Masiulaniec <jacekm@dobremiasto.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/tree.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <event.h>
#include <fcntl.h>
#include <imsg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "smtpd.h"
#include "log.h"

#define	QWALK_AGAIN	0x1
#define	QWALK_RECURSE	0x2
#define	QWALK_RETURN	0x3

struct qwalk {
	char	  path[MAXPATHLEN];
	DIR	 *dirs[3];
	int	(*filefn)(struct qwalk *, char *);
	int	  bucket;
	int	  level;
	int	  strict;
};

int		walk_simple(struct qwalk *, char *);
int		walk_queue(struct qwalk *, char *);

void		display_envelope(struct message *, int);
void		getflag(u_int *, int, char *, char *, size_t);

void
queue_delete_layout_message(char *queuepath, char *msgid)
{
	char rootdir[MAXPATHLEN];
	char purgedir[MAXPATHLEN];

	if (! bsnprintf(rootdir, sizeof(rootdir), "%s/%s", queuepath, msgid))
		fatalx("snprintf");

	if (! bsnprintf(purgedir, sizeof(purgedir), "%s/%s", PATH_PURGE, msgid))
		fatalx("snprintf");

	if (rename(rootdir, purgedir) == -1)
		fatal("queue_delete_layout_message: rename");
}

int
queue_record_layout_envelope(char *queuepath, struct message *message)
{
	char evpname[MAXPATHLEN];
	FILE *fp;
	int fd;

	fp = NULL;

again:
	if (! bsnprintf(evpname, sizeof(evpname), "%s/%s%s/%s.%qu", queuepath,
		message->message_id, PATH_ENVELOPES, message->message_id,
		(u_int64_t)arc4random()))
		fatalx("queue_record_incoming_envelope: snprintf");

	fd = open(evpname, O_WRONLY|O_CREAT|O_EXCL, 0600);
	if (fd == -1) {
		if (errno == EEXIST)
			goto again;
		if (errno == ENOSPC || errno == ENFILE)
			goto tempfail;
		fatal("queue_record_incoming_envelope: open");
	}

	fp = fdopen(fd, "w");
	if (fp == NULL)
		fatal("queue_record_incoming_envelope: fdopen");

	message->creation = time(NULL);
	if (strlcpy(message->message_uid, strrchr(evpname, '/') + 1,
	    sizeof(message->message_uid)) >= sizeof(message->message_uid))
		fatalx("queue_record_incoming_envelope: truncation");

	if (fwrite(message, sizeof (struct message), 1, fp) != 1) {
		if (errno == ENOSPC)
			goto tempfail;
		fatal("queue_record_incoming_envelope: write");
	}

	if (! safe_fclose(fp)) {
		fp = NULL;
		fd = -1;
		goto tempfail;
	}

	return 1;

tempfail:
	unlink(evpname);
	if (fp)
		fclose(fp);
	else if (fd != -1)
		close(fd);
	message->creation = 0;
	message->message_uid[0] = '\0';

	return 0;
}

int
queue_remove_layout_envelope(char *queuepath, struct message *message)
{
	char pathname[MAXPATHLEN];

	if (! bsnprintf(pathname, sizeof(pathname), "%s/%s%s/%s", queuepath,
		message->message_id, PATH_ENVELOPES, message->message_uid))
		fatal("queue_remove_incoming_envelope: snprintf");

	if (unlink(pathname) == -1)
		fatal("queue_remove_incoming_envelope: unlink");

	return 1;
}

void
enqueue_delete_message(char *msgid)
{
	queue_delete_layout_message(PATH_ENQUEUE, msgid);
}

int
enqueue_record_envelope(struct message *message)
{
	return queue_record_layout_envelope(PATH_ENQUEUE, message);
}

int
enqueue_remove_envelope(struct message *message)
{
	return queue_remove_layout_envelope(PATH_ENQUEUE, message);
}

void
bounce_delete_message(char *msgid)
{
	queue_delete_layout_message(PATH_BOUNCE, msgid);
}

int
bounce_record_envelope(struct message *message)
{
	message->lasttry = 0;
	message->retry = 0;
	return queue_record_layout_envelope(PATH_BOUNCE, message);
}

int
bounce_remove_envelope(struct message *message)
{
	return queue_remove_layout_envelope(PATH_BOUNCE, message);
}

int
bounce_record_message(struct smtpd *env, struct message *messagep, struct message *mbounce)
{
	char	msgid[MAX_ID_SIZE];

	if (messagep->type == T_BOUNCE_MESSAGE) {
		log_debug("mailer daemons loop detected !");
		return 0;
	}

	*mbounce = *messagep;
	mbounce->type = T_BOUNCE_MESSAGE;
	mbounce->status &= ~S_MESSAGE_PERMFAILURE;

	strlcpy(msgid, messagep->message_id, sizeof(msgid));
	if (! queue_message_create(env, Q_BOUNCE, msgid))
		return 0;

	strlcpy(mbounce->message_id, msgid, sizeof(mbounce->message_id));
	if (! bounce_record_envelope(mbounce))
		return 0;

	return queue_message_commit(env, Q_BOUNCE, msgid);
}

void
queue_delete_incoming_message(char *msgid)
{
	queue_delete_layout_message(PATH_INCOMING, msgid);
}

int
queue_record_incoming_envelope(struct message *message)
{
	return queue_record_layout_envelope(PATH_INCOMING, message);
}

int
queue_remove_incoming_envelope(struct message *message)
{
	return queue_remove_layout_envelope(PATH_INCOMING, message);
}

void
queue_message_update(struct smtpd *env, struct message *messagep)
{
	messagep->flags &= ~F_MESSAGE_PROCESSING;
	messagep->status &= ~(S_MESSAGE_ACCEPTED|S_MESSAGE_REJECTED);
	messagep->batch_id = 0;
	messagep->retry++;

	if (messagep->status & S_MESSAGE_PERMFAILURE) {
		if (messagep->type != T_BOUNCE_MESSAGE &&
		    messagep->sender.user[0] != '\0') {
			struct message bounce;

			bounce_record_message(env, messagep, &bounce);
		}
		queue_envelope_delete(env, Q_QUEUE, messagep);
		return;
	}

	if (messagep->status & S_MESSAGE_TEMPFAILURE) {
		messagep->status &= ~S_MESSAGE_TEMPFAILURE;
		queue_envelope_update(env, Q_QUEUE, messagep);
		return;
	}

	/* no error, remove envelope */
	queue_envelope_delete(env, Q_QUEUE, messagep);
}

u_int16_t
queue_hash(char *msgid)
{
	u_int16_t	h;

	for (h = 5381; *msgid; msgid++)
		h = ((h << 5) + h) + *msgid;

	return (h % DIRHASH_BUCKETS);
}

struct qwalk *
qwalk_new(char *path)
{
	struct qwalk *q;

	q = calloc(1, sizeof(struct qwalk));
	if (q == NULL)
		fatal("qwalk_new: calloc");

	strlcpy(q->path, path, sizeof(q->path));

	q->level = 0;
	q->strict = 0;
	q->filefn = walk_simple;

	if (smtpd_process == PROC_QUEUE || smtpd_process == PROC_RUNNER)
		q->strict = 1;

	if (strcmp(path, PATH_QUEUE) == 0)
		q->filefn = walk_queue;

	q->dirs[0] = opendir(q->path);
	if (q->dirs[0] == NULL)
		fatal("qwalk_new: opendir");

	return (q);
}

int
qwalk(struct qwalk *q, char *filepath)
{
	struct dirent	*dp;

again:
	errno = 0;
	dp = readdir(q->dirs[q->level]);
	if (errno)
		fatal("qwalk: readdir");
	if (dp == NULL) {
		closedir(q->dirs[q->level]);
		q->dirs[q->level] = NULL;
		if (q->level == 0)
			return (0);
		q->level--;
		goto again;
	}

	if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
		goto again;

	switch (q->filefn(q, dp->d_name)) {
	case QWALK_AGAIN:
		goto again;
	case QWALK_RECURSE:
		goto recurse;
	case QWALK_RETURN:
		if (! bsnprintf(filepath, MAXPATHLEN, "%s/%s", q->path,
			dp->d_name))
			fatalx("qwalk: snprintf");
		return (1);
	default:
		fatalx("qwalk: callback failed");
	}

recurse:
	q->level++;
	q->dirs[q->level] = opendir(q->path);
	if (q->dirs[q->level] == NULL) {
		if (errno == ENOENT && !q->strict) {
			q->level--;
			goto again;
		}
		fatal("qwalk: opendir");
	}
	goto again;
}

void
qwalk_close(struct qwalk *q)
{
	int i;

	for (i = 0; i <= q->level; i++)
		if (q->dirs[i])
			closedir(q->dirs[i]);

	bzero(q, sizeof(struct qwalk));
	free(q);
}

int
walk_simple(struct qwalk *q, char *fname)
{
	return (QWALK_RETURN);
}

int
walk_queue(struct qwalk *q, char *fname)
{
	const char	*errstr;

	switch (q->level) {
	case 0:
		if (strcmp(fname, "envelope.tmp") == 0)
			return (QWALK_AGAIN);
		q->bucket = strtonum(fname, 0, DIRHASH_BUCKETS - 1, &errstr);
		if (errstr) {
			log_warnx("walk_queue: invalid bucket: %s", fname);
			return (QWALK_AGAIN);
		}
		if (! bsnprintf(q->path, sizeof(q->path), "%s/%d", PATH_QUEUE,
			q->bucket))
			fatalx("walk_queue: snprintf");
		return (QWALK_RECURSE);
	case 1:
		if (! bsnprintf(q->path, sizeof(q->path), "%s/%d/%s%s",
			PATH_QUEUE, q->bucket, fname, PATH_ENVELOPES))
			fatalx("walk_queue: snprintf");
		return (QWALK_RECURSE);
	case 2:
		return (QWALK_RETURN);
	}

	return (-1);
}

void
show_queue(char *queuepath, int flags)
{
	char		 path[MAXPATHLEN];
	struct message	 message;
	struct qwalk	*q;
	FILE		*fp;

	log_init(1);

	if (chroot(PATH_SPOOL) == -1 || chdir(".") == -1)
		err(1, "%s", PATH_SPOOL);

	q = qwalk_new(queuepath);

	while (qwalk(q, path)) {
		fp = fopen(path, "r");
		if (fp == NULL) {
			if (errno == ENOENT)
				continue;
			err(1, "%s", path);
		}

		errno = 0;
		if (fread(&message, sizeof(struct message), 1, fp) != 1)
			err(1, "%s", path);
		fclose(fp);

		display_envelope(&message, flags);
	}

	qwalk_close(q);
}

void
display_envelope(struct message *envelope, int flags)
{
	char	 status[128];

	status[0] = '\0';

	getflag(&envelope->status, S_MESSAGE_TEMPFAILURE, "TEMPFAIL",
	    status, sizeof(status));

	if (envelope->status)
		errx(1, "%s: unexpected status 0x%04x", envelope->message_uid,
		    envelope->status);

	getflag(&envelope->flags, F_MESSAGE_BOUNCE, "BOUNCE",
	    status, sizeof(status));
	getflag(&envelope->flags, F_MESSAGE_AUTHENTICATED, "AUTH",
	    status, sizeof(status));
	getflag(&envelope->flags, F_MESSAGE_PROCESSING, "PROCESSING",
	    status, sizeof(status));
	getflag(&envelope->flags, F_MESSAGE_SCHEDULED, "SCHEDULED",
	    status, sizeof(status));
	getflag(&envelope->flags, F_MESSAGE_ENQUEUED, "ENQUEUED",
	    status, sizeof(status));
	getflag(&envelope->flags, F_MESSAGE_FORCESCHEDULE, "SCHEDULED_MANUAL",
	    status, sizeof(status));

	if (envelope->flags)
		errx(1, "%s: unexpected flags 0x%04x", envelope->message_uid,
		    envelope->flags);
	
	if (status[0])
		status[strlen(status) - 1] = '\0';
	else
		strlcpy(status, "-", sizeof(status));

	switch (envelope->type) {
	case T_MDA_MESSAGE:
		printf("MDA");
		break;
	case T_MTA_MESSAGE:
		printf("MTA");
		break;
	case T_BOUNCE_MESSAGE:
		printf("BOUNCE");
		break;
	default:
		printf("UNKNOWN");
	}
	
	printf("|%s|%s|%s@%s|%s@%s|%d|%d|%u",
	    envelope->message_uid,
	    status,
	    envelope->sender.user, envelope->sender.domain,
	    envelope->recipient.user, envelope->recipient.domain,
	    envelope->lasttry,
	    envelope->expire,
	    envelope->retry);
	
	if (envelope->session_errorline[0] != '\0')
		printf("|%s", envelope->session_errorline);

	printf("\n");
}

void
getflag(u_int *bitmap, int bit, char *bitstr, char *buf, size_t len)
{
	if (*bitmap & bit) {
		*bitmap &= ~bit;
		strlcat(buf, bitstr, len);
		strlcat(buf, ",", len);
	}
}
