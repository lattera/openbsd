/* $OpenBSD: src/usr.bin/ssh/Attic/cli.h,v 1.2.2.1 2001/02/16 20:12:58 jason Exp $ */

#ifndef CLI_H
#define CLI_H

/*
 * Presents a prompt and returns the response allocated with xmalloc().
 * Uses /dev/tty or stdin/out depending on arg.  Optionally disables echo
 * of response depending on arg.  Tries to ensure that no other userland
 * buffer is storing the response.
 */
char *	cli_read_passphrase(char * prompt, int from_stdin, int echo_enable);
char *	cli_prompt(char * prompt, int echo_enable);
void	cli_mesg(char * mesg);

#endif /* CLI_H */
