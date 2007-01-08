/*	$OpenBSD: src/usr.sbin/relayd/control.c,v 1.6 2007/01/08 20:46:18 reyk Exp $	*/

/*
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
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

#include <sys/queue.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <net/if.h>
#include <errno.h>
#include <event.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "hostated.h"

#define	CONTROL_BACKLOG	5

struct ctl_connlist ctl_conns;

int control_imsg_relay(struct imsg *imsg);

struct ctl_conn	*control_connbyfd(int);
struct ctl_conn	*control_connbypid(pid_t);
void		 control_close(int);

int
control_init(void)
{
	struct sockaddr_un	 sun;
	int			 fd;
	mode_t			 old_umask;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		log_warn("control_init: socket");
		return (-1);
	}

	bzero(&sun, sizeof(sun));
	sun.sun_family = AF_UNIX;
	strlcpy(sun.sun_path, HOSTATED_SOCKET, sizeof(sun.sun_path));

	if (unlink(HOSTATED_SOCKET) == -1)
		if (errno != ENOENT) {
			log_warn("control_init: unlink %s", HOSTATED_SOCKET);
			close(fd);
			return (-1);
		}

	old_umask = umask(S_IXUSR|S_IXGRP|S_IWOTH|S_IROTH|S_IXOTH);
	if (bind(fd, (struct sockaddr *)&sun, sizeof(sun)) == -1) {
		log_warn("control_init: bind: %s", HOSTATED_SOCKET);
		close(fd);
		umask(old_umask);
		return (-1);
	}
	umask(old_umask);

	if (chmod(HOSTATED_SOCKET, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP) == -1) {
		log_warn("control_init: chmod");
		close(fd);
		(void)unlink(HOSTATED_SOCKET);
		return (-1);
	}

	session_socket_blockmode(fd, BM_NONBLOCK);
	control_state.fd = fd;

	return (0);
}

int
control_listen(void)
{

	if (listen(control_state.fd, CONTROL_BACKLOG) == -1) {
		log_warn("control_listen: listen");
		return (-1);
	}

	event_set(&control_state.ev, control_state.fd, EV_READ | EV_PERSIST,
	    control_accept, NULL);
	event_add(&control_state.ev, NULL);

	return (0);
}

void
control_cleanup(void)
{

	unlink(HOSTATED_SOCKET);
}

/* ARGSUSED */
void
control_accept(int listenfd, short event, void *arg)
{
	int			 connfd;
	socklen_t		 len;
	struct sockaddr_un	 sun;
	struct ctl_conn		*c;

	len = sizeof(sun);
	if ((connfd = accept(listenfd,
	    (struct sockaddr *)&sun, &len)) == -1) {
		if (errno != EWOULDBLOCK && errno != EINTR)
			log_warn("control_accept");
		return;
	}

	session_socket_blockmode(connfd, BM_NONBLOCK);

	if ((c = malloc(sizeof(struct ctl_conn))) == NULL) {
		log_warn("control_accept");
		return;
	}

	imsg_init(&c->ibuf, connfd, control_dispatch_imsg);
	c->ibuf.events = EV_READ;
	event_set(&c->ibuf.ev, c->ibuf.fd, c->ibuf.events,
	    c->ibuf.handler, &c->ibuf);
	event_add(&c->ibuf.ev, NULL);

	TAILQ_INSERT_TAIL(&ctl_conns, c, entry);
}

struct ctl_conn *
control_connbyfd(int fd)
{
	struct ctl_conn	*c;

	for (c = TAILQ_FIRST(&ctl_conns); c != NULL && c->ibuf.fd != fd;
	    c = TAILQ_NEXT(c, entry))
		;	/* nothing */

	return (c);
}

struct ctl_conn *
control_connbypid(pid_t pid)
{
	struct ctl_conn	*c;

	for (c = TAILQ_FIRST(&ctl_conns); c != NULL && c->ibuf.pid != pid;
	    c = TAILQ_NEXT(c, entry))
		;	/* nothing */

	return (c);
}

void
control_close(int fd)
{
	struct ctl_conn	*c;

	if ((c = control_connbyfd(fd)) == NULL)
		log_warn("control_close: fd %d: not found", fd);

	msgbuf_clear(&c->ibuf.w);
	TAILQ_REMOVE(&ctl_conns, c, entry);

	event_del(&c->ibuf.ev);
	close(c->ibuf.fd);
	free(c);
}

/* ARGSUSED */
void
control_dispatch_imsg(int fd, short event, void *arg)
{
	struct ctl_conn		*c;
	struct imsg		 imsg;
	struct ctl_id		 id;
	int			 n;

	if ((c = control_connbyfd(fd)) == NULL) {
		log_warn("control_dispatch_imsg: fd %d: not found", fd);
		return;
	}

	switch (event) {
	case EV_READ:
		if ((n = imsg_read(&c->ibuf)) <= 0) {
			control_close(fd);
			return;
		}
		break;
	case EV_WRITE:
		if (msgbuf_write(&c->ibuf.w) < 0) {
			control_close(fd);
			return;
		}
		imsg_event_add(&c->ibuf);
		return;
	default:
		fatalx("unknown event");
	}

	for (;;) {
		if ((n = imsg_get(&c->ibuf, &imsg)) == -1) {
			control_close(fd);
			return;
		}

		if (n == 0)
			break;

		switch (imsg.hdr.type) {
		case IMSG_CTL_SHOW_SUM:
			show(c);
			break;
		case IMSG_CTL_SERVICE_DISABLE:
			if (imsg.hdr.len != IMSG_HEADER_SIZE + sizeof(id))
				fatalx("invalid imsg header len");
			memcpy(&id, imsg.data, sizeof(id));
			if (disable_service(c, &id))
				imsg_compose(&c->ibuf, IMSG_CTL_FAIL, 0, 0,
				    NULL, 0);
			else
				imsg_compose(&c->ibuf, IMSG_CTL_OK, 0, 0,
				    NULL, 0);
			break;
		case IMSG_CTL_SERVICE_ENABLE:
			if (imsg.hdr.len != IMSG_HEADER_SIZE + sizeof(id))
				fatalx("invalid imsg header len");
			memcpy(&id, imsg.data, sizeof(id));
			if (enable_service(c, &id))
				imsg_compose(&c->ibuf, IMSG_CTL_FAIL, 0, 0,
				    NULL, 0);
			else
				imsg_compose(&c->ibuf, IMSG_CTL_OK, 0, 0,
				    NULL, 0);
			break;
		case IMSG_CTL_TABLE_DISABLE:
			if (imsg.hdr.len != IMSG_HEADER_SIZE + sizeof(id))
				fatalx("invalid imsg header len");
			memcpy(&id, imsg.data, sizeof(id));
			if (disable_table(c, &id))
				imsg_compose(&c->ibuf, IMSG_CTL_FAIL, 0, 0,
				    NULL, 0);
			else
				imsg_compose(&c->ibuf, IMSG_CTL_OK, 0, 0,
				    NULL, 0);
			break;
		case IMSG_CTL_TABLE_ENABLE:
			if (imsg.hdr.len != IMSG_HEADER_SIZE + sizeof(id))
				fatalx("invalid imsg header len");
			memcpy(&id, imsg.data, sizeof(id));
			if (enable_table(c, &id))
				imsg_compose(&c->ibuf, IMSG_CTL_FAIL, 0, 0,
				    NULL, 0);
			else
				imsg_compose(&c->ibuf, IMSG_CTL_OK, 0, 0,
				    NULL, 0);
			break;
		case IMSG_CTL_HOST_DISABLE:
			if (imsg.hdr.len != IMSG_HEADER_SIZE + sizeof(id))
				fatalx("invalid imsg header len");
			memcpy(&id, imsg.data, sizeof(id));
			if (disable_host(c, &id))
				imsg_compose(&c->ibuf, IMSG_CTL_FAIL, 0, 0,
				    NULL, 0);
			else
				imsg_compose(&c->ibuf, IMSG_CTL_OK, 0, 0,
				    NULL, 0);
			break;
		case IMSG_CTL_HOST_ENABLE:
			if (imsg.hdr.len != IMSG_HEADER_SIZE + sizeof(id))
				fatalx("invalid imsg header len");
			memcpy(&id, imsg.data, sizeof(id));
			if (enable_host(c, &id))
				imsg_compose(&c->ibuf, IMSG_CTL_FAIL, 0, 0,
				    NULL, 0);
			else
				imsg_compose(&c->ibuf, IMSG_CTL_OK, 0, 0,
				    NULL, 0);
			break;
		case IMSG_CTL_SHUTDOWN:
		case IMSG_CTL_RELOAD:
			imsg_compose(&c->ibuf, IMSG_CTL_FAIL, 0, 0, NULL, 0);
			break;
		default:
			log_debug("control_dispatch_imsg: "
			    "error handling imsg %d", imsg.hdr.type);
			break;
		}
		imsg_free(&imsg);
	}

	imsg_event_add(&c->ibuf);
}

int
control_imsg_relay(struct imsg *imsg)
{
	struct ctl_conn	*c;

	if ((c = control_connbypid(imsg->hdr.pid)) == NULL)
		return (0);

	return (imsg_compose(&c->ibuf, imsg->hdr.type, 0, imsg->hdr.pid,
	    imsg->data, imsg->hdr.len - IMSG_HEADER_SIZE));
}

void
session_socket_blockmode(int fd, enum blockmodes bm)
{
	int	flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
		fatal("fnctl F_GETFL");

	if (bm == BM_NONBLOCK)
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;

	if ((flags = fcntl(fd, F_SETFL, flags)) == -1)
		fatal("fnctl F_SETFL");
}
