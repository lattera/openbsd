/*
 * $Id: server.h,v 1.3 1997/10/26 01:03:40 brian Exp $
 */

extern int server;

extern int ServerLocalOpen(const char *, mode_t);
extern int ServerTcpOpen(int);
extern void ServerClose(void);
