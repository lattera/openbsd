/* $OpenBSD: src/usr.bin/tmux/cmd-kill-server.c,v 1.5 2009/11/13 19:53:29 nicm Exp $ */

/*
 * Copyright (c) 2007 Nicholas Marriott <nicm@users.sourceforge.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>

#include <signal.h>
#include <unistd.h>

#include "tmux.h"

/*
 * Kill the server and do nothing else.
 */

int	cmd_kill_server_exec(struct cmd *, struct cmd_ctx *);

const struct cmd_entry cmd_kill_server_entry = {
	"kill-server", NULL,
	"",
	0, "",
	NULL,
	NULL,
	cmd_kill_server_exec,
	NULL,
	NULL
};

int
cmd_kill_server_exec(unused struct cmd *self, unused struct cmd_ctx *ctx)
{
	kill(getpid(), SIGTERM);

	return (0);
}
