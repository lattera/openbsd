/*	$OpenBSD: src/sys/netinet/tcp_var.h,v 1.48 2003/05/26 05:01:55 itojun Exp $	*/
/*	$NetBSD: tcp_var.h,v 1.17 1996/02/13 23:44:24 christos Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)tcp_var.h	8.3 (Berkeley) 4/10/94
 */

#ifndef _NETINET_TCP_VAR_H_
#define _NETINET_TCP_VAR_H_

struct sackblk {
	tcp_seq start;		/* start seq no. of sack block */
	tcp_seq end; 		/* end seq no. */
};

struct sackhole {
	tcp_seq start;		/* start seq no. of hole */
	tcp_seq end;		/* end seq no. */
	int	dups;		/* number of dup(s)acks for this hole */
	tcp_seq rxmit;		/* next seq. no in hole to be retransmitted */
	struct sackhole *next;	/* next in list */
};

/*
 * Kernel variables for tcp.
 */

/*
 * Tcp control block, one per tcp; fields:
 */
struct tcpcb {
	struct ipqehead segq;		/* sequencing queue */
	struct timeout t_timer[TCPT_NTIMERS];	/* tcp timers */
	short	t_state;		/* state of this connection */
	short	t_rxtshift;		/* log(2) of rexmt exp. backoff */
	short	t_rxtcur;		/* current retransmit value */
	short	t_dupacks;		/* consecutive dup acks recd */
	u_short	t_maxseg;		/* maximum segment size */
	char	t_force;		/* 1 if forcing out a byte */
	u_int	t_flags;
#define	TF_ACKNOW	0x0001		/* ack peer immediately */
#define	TF_DELACK	0x0002		/* ack, but try to delay it */
#define	TF_NODELAY	0x0004		/* don't delay packets to coalesce */
#define	TF_NOOPT	0x0008		/* don't use tcp options */
#define	TF_SENTFIN	0x0010		/* have sent FIN */
#define	TF_REQ_SCALE	0x0020		/* have/will request window scaling */
#define	TF_RCVD_SCALE	0x0040		/* other side has requested scaling */
#define	TF_REQ_TSTMP	0x0080		/* have/will request timestamps */
#define	TF_RCVD_TSTMP	0x0100		/* a timestamp was received in SYN */
#define	TF_SACK_PERMIT	0x0200		/* other side said I could SACK */
#define	TF_SIGNATURE	0x0400		/* require TCP MD5 signature */
#ifdef TCP_ECN
#define TF_ECN_PERMIT	0x00008000	/* other side said I could ECN */
#define TF_RCVD_CE	0x00010000	/* send ECE in subsequent segs */
#define TF_SEND_CWR	0x00020000	/* send CWR in next seg */
#define TF_DISABLE_ECN	0x00040000	/* disable ECN for this connection */
#endif

	struct	mbuf *t_template;	/* skeletal packet for transmit */
	struct	inpcb *t_inpcb;		/* back pointer to internet pcb */
	struct	timeout t_delack_to;	/* delayed ACK callback */
/*
 * The following fields are used as in the protocol specification.
 * See RFC783, Dec. 1981, page 21.
 */
/* send sequence variables */
	tcp_seq	snd_una;		/* send unacknowledged */
	tcp_seq	snd_nxt;		/* send next */
	tcp_seq	snd_up;			/* send urgent pointer */
	tcp_seq	snd_wl1;		/* window update seg seq number */
	tcp_seq	snd_wl2;		/* window update seg ack number */
	tcp_seq	iss;			/* initial send sequence number */
	u_long	snd_wnd;		/* send window */
#if 1 /*def TCP_SACK*/
	int	sack_disable;		/* disable SACK for this connection */
	int	snd_numholes;		/* number of holes seen by sender */
	struct sackhole *snd_holes;	/* linked list of holes (sorted) */
#if 1 /*defined(TCP_SACK) && defined(TCP_FACK)*/
	tcp_seq snd_fack;		/* for FACK congestion control */
	u_long	snd_awnd;		/* snd_nxt - snd_fack + */
					/* retransmitted data */
	int retran_data;		/* amount of outstanding retx. data  */
#endif /* TCP_FACK */
#endif /* TCP_SACK */
#if 1 /*defined(TCP_SACK) || defined(TCP_ECN)*/
	tcp_seq snd_last;		/* for use in fast recovery */
#endif
/* receive sequence variables */
	u_long	rcv_wnd;		/* receive window */
	tcp_seq	rcv_nxt;		/* receive next */
	tcp_seq	rcv_up;			/* receive urgent pointer */
	tcp_seq	irs;			/* initial receive sequence number */
#if 1 /*def TCP_SACK*/
	tcp_seq rcv_laststart;		/* start of last segment recd. */
	tcp_seq rcv_lastend;		/* end of ... */
	tcp_seq rcv_lastsack;		/* last seq number(+1) sack'd by rcv'r*/
	int	rcv_numsacks;		/* # distinct sack blks present */
	struct sackblk sackblks[MAX_SACK_BLKS]; /* seq nos. of sack blocks */
#endif

/*
 * Additional variables for this implementation.
 */
/* receive variables */
	tcp_seq	rcv_adv;		/* advertised window */
/* retransmit variables */
	tcp_seq	snd_max;		/* highest sequence number sent;
					 * used to recognize retransmits
					 */
/* congestion control (for slow start, source quench, retransmit after loss) */
	u_long	snd_cwnd;		/* congestion-controlled window */
	u_long	snd_ssthresh;		/* snd_cwnd size threshhold for
					 * for slow start exponential to
					 * linear switch
					 */
	u_short	t_maxopd;		/* mss plus options */
	u_short	t_peermss;		/* peer's maximum segment size */

/*
 * transmit timing stuff.  See below for scale of srtt and rttvar.
 * "Variance" is actually smoothed difference.
 */
	uint32_t t_rcvtime;		/* time last segment received */
	uint32_t t_rtttime;		/* time we started measuring rtt */
	tcp_seq	t_rtseq;		/* sequence number being timed */
	short	t_srtt;			/* smoothed round-trip time */
	short	t_rttvar;		/* variance in round-trip time */
	u_short	t_rttmin;		/* minimum rtt allowed */
	u_long	max_sndwnd;		/* largest window peer has offered */

/* out-of-band data */
	char	t_oobflags;		/* have some */
	char	t_iobc;			/* input character */
#define	TCPOOB_HAVEDATA	0x01
#define	TCPOOB_HADDATA	0x02
	short	t_softerror;		/* possible error not yet reported */

/* RFC 1323 variables */
	u_char	snd_scale;		/* window scaling for send window */
	u_char	rcv_scale;		/* window scaling for recv window */
	u_char	request_r_scale;	/* pending window scaling */
	u_char	requested_s_scale;
	u_int32_t ts_recent;		/* timestamp echo data */
	u_int32_t ts_recent_age;		/* when last updated */
	tcp_seq	last_ack_sent;

/* TUBA stuff */
	caddr_t	t_tuba_pcb;		/* next level down pcb for TCP over z */

	int pf;
};

#define	intotcpcb(ip)	((struct tcpcb *)(ip)->inp_ppcb)
#define	sototcpcb(so)	(intotcpcb(sotoinpcb(so)))

#ifdef _KERNEL
extern int tcp_delack_ticks;
void	tcp_delack(void *);

#define TCP_INIT_DELACK(tp)						\
	timeout_set(&(tp)->t_delack_to, tcp_delack, tp)

#define TCP_RESTART_DELACK(tp)						\
	timeout_add(&(tp)->t_delack_to, tcp_delack_ticks)

#define	TCP_SET_DELACK(tp)						\
do {									\
	if (((tp)->t_flags & TF_DELACK) == 0) {				\
		(tp)->t_flags |= TF_DELACK;				\
		TCP_RESTART_DELACK(tp);					\
	}								\
} while (/*CONSTCOND*/0)

#define	TCP_CLEAR_DELACK(tp)						\
do {									\
	if ((tp)->t_flags & TF_DELACK) {				\
		(tp)->t_flags &= ~TF_DELACK;				\
		timeout_del(&(tp)->t_delack_to);			\
	}								\
} while (/*CONSTCOND*/0)
#endif /* _KERNEL */

/*
 * The smoothed round-trip time and estimated variance
 * are stored as fixed point numbers scaled by the values below.
 * For convenience, these scales are also used in smoothing the average
 * (smoothed = (1/scale)sample + ((scale-1)/scale)smoothed).
 * With these scales, srtt has 3 bits to the right of the binary point,
 * and thus an "ALPHA" of 0.875.  rttvar has 2 bits to the right of the
 * binary point, and is smoothed with an ALPHA of 0.75.
 */
#define	TCP_RTT_SCALE		8	/* multiplier for srtt; 3 bits frac. */
#define	TCP_RTT_SHIFT		3	/* shift for srtt; 3 bits frac. */
#define	TCP_RTTVAR_SCALE	4	/* multiplier for rttvar; 2 bits */
#define	TCP_RTTVAR_SHIFT	2	/* multiplier for rttvar; 2 bits */

/*
 * The initial retransmission should happen at rtt + 4 * rttvar.
 * Because of the way we do the smoothing, srtt and rttvar
 * will each average +1/2 tick of bias.  When we compute
 * the retransmit timer, we want 1/2 tick of rounding and
 * 1 extra tick because of +-1/2 tick uncertainty in the
 * firing of the timer.  The bias will give us exactly the
 * 1.5 tick we need.  But, because the bias is
 * statistical, we have to test that we don't drop below
 * the minimum feasible timer (which is 2 ticks).
 * This macro assumes that the value of TCP_RTTVAR_SCALE
 * is the same as the multiplier for rttvar.
 */
#define	TCP_REXMTVAL(tp) \
	((((tp)->t_srtt >> TCP_RTT_SHIFT) + (tp)->t_rttvar) >> 2)

/*
 * TCP statistics.
 * Many of these should be kept per connection,
 * but that's inconvenient at the moment.
 */
struct	tcpstat {
	u_int32_t tcps_connattempt;	/* connections initiated */
	u_int32_t tcps_accepts;		/* connections accepted */
	u_int32_t tcps_connects;	/* connections established */
	u_int32_t tcps_drops;		/* connections dropped */
	u_int32_t tcps_conndrops;	/* embryonic connections dropped */
	u_int32_t tcps_closed;		/* conn. closed (includes drops) */
	u_int32_t tcps_segstimed;	/* segs where we tried to get rtt */
	u_int32_t tcps_rttupdated;	/* times we succeeded */
	u_int32_t tcps_delack;		/* delayed acks sent */
	u_int32_t tcps_timeoutdrop;	/* conn. dropped in rxmt timeout */
	u_int32_t tcps_rexmttimeo;	/* retransmit timeouts */
	u_int32_t tcps_persisttimeo;	/* persist timeouts */
	u_int32_t tcps_persistdrop;	/* connections dropped in persist */
	u_int32_t tcps_keeptimeo;	/* keepalive timeouts */
	u_int32_t tcps_keepprobe;	/* keepalive probes sent */
	u_int32_t tcps_keepdrops;	/* connections dropped in keepalive */

	u_int32_t tcps_sndtotal;		/* total packets sent */
	u_int32_t tcps_sndpack;		/* data packets sent */
	u_int64_t tcps_sndbyte;		/* data bytes sent */
	u_int32_t tcps_sndrexmitpack;	/* data packets retransmitted */
	u_int64_t tcps_sndrexmitbyte;	/* data bytes retransmitted */
	u_int64_t tcps_sndrexmitfast;	/* Fast retransmits */
	u_int32_t tcps_sndacks;		/* ack-only packets sent */
	u_int32_t tcps_sndprobe;	/* window probes sent */
	u_int32_t tcps_sndurg;		/* packets sent with URG only */
	u_int32_t tcps_sndwinup;	/* window update-only packets sent */
	u_int32_t tcps_sndctrl;		/* control (SYN|FIN|RST) packets sent */

	u_int32_t tcps_rcvtotal;	/* total packets received */
	u_int32_t tcps_rcvpack;		/* packets received in sequence */
	u_int64_t tcps_rcvbyte;		/* bytes received in sequence */
	u_int32_t tcps_rcvbadsum;	/* packets received with ccksum errs */
	u_int32_t tcps_rcvbadoff;	/* packets received with bad offset */
	u_int32_t tcps_rcvmemdrop;	/* packets dropped for lack of memory */
	u_int32_t tcps_rcvnosec;	/* packets dropped for lack of ipsec */
	u_int32_t tcps_rcvshort;	/* packets received too short */
	u_int32_t tcps_rcvduppack;	/* duplicate-only packets received */
	u_int64_t tcps_rcvdupbyte;	/* duplicate-only bytes received */
	u_int32_t tcps_rcvpartduppack;	/* packets with some duplicate data */
	u_int64_t tcps_rcvpartdupbyte;	/* dup. bytes in part-dup. packets */
	u_int32_t tcps_rcvoopack;	/* out-of-order packets received */
	u_int64_t tcps_rcvoobyte;	/* out-of-order bytes received */
	u_int32_t tcps_rcvpackafterwin;	/* packets with data after window */
	u_int64_t tcps_rcvbyteafterwin;	/* bytes rcvd after window */
	u_int32_t tcps_rcvafterclose;	/* packets rcvd after "close" */
	u_int32_t tcps_rcvwinprobe;	/* rcvd window probe packets */
	u_int32_t tcps_rcvdupack;	/* rcvd duplicate acks */
	u_int32_t tcps_rcvacktoomuch;	/* rcvd acks for unsent data */
	u_int32_t tcps_rcvackpack;	/* rcvd ack packets */
	u_int64_t tcps_rcvackbyte;	/* bytes acked by rcvd acks */
	u_int32_t tcps_rcvwinupd;	/* rcvd window update packets */
	u_int32_t tcps_pawsdrop;	/* segments dropped due to PAWS */
	u_int32_t tcps_predack;		/* times hdr predict ok for acks */
	u_int32_t tcps_preddat;		/* times hdr predict ok for data pkts */

	u_int32_t tcps_pcbhashmiss;	/* input packets missing pcb hash */
	u_int32_t tcps_noport;		/* no socket on port */
	u_int32_t tcps_badsyn;		/* SYN packet with src==dst rcv'ed */

	u_int32_t tcps_rcvbadsig;	/* rcvd bad/missing TCP signatures */
	u_int64_t tcps_rcvgoodsig;	/* rcvd good TCP signatures */
	u_int32_t tcps_inhwcsum;	/* input hardware-checksummed packets */
	u_int32_t tcps_outhwcsum;	/* output hardware-checksummed packets */

	/* ECN stats */
	u_int32_t tcps_ecn_accepts;	/* ecn connections accepted */
	u_int32_t tcps_ecn_rcvece;	/* # of rcvd ece */
	u_int32_t tcps_ecn_rcvcwr;	/* # of rcvd cwr */
	u_int32_t tcps_ecn_rcvce;	/* # of rcvd ce in ip header */
	u_int32_t tcps_ecn_sndect;	/* # of cwr sent */
	u_int32_t tcps_ecn_sndece;	/* # of ece sent */
	u_int32_t tcps_ecn_sndcwr;	/* # of cwr sent */
	u_int32_t tcps_cwr_ecn;		/* # of cwnd reduced by ecn */
	u_int32_t tcps_cwr_frecovery;	/* # of cwnd reduced by fastrecovery */
	u_int32_t tcps_cwr_timeout;	/* # of cwnd reduced by timeout */
};

/*
 * Names for TCP sysctl objects.
 */

#define	TCPCTL_RFC1323		1 /* enable/disable RFC1323 timestamps/scaling */
#define	TCPCTL_KEEPINITTIME	2 /* TCPT_KEEP value */
#define TCPCTL_KEEPIDLE		3 /* allow tcp_keepidle to be changed */
#define TCPCTL_KEEPINTVL	4 /* allow tcp_keepintvl to be changed */
#define TCPCTL_SLOWHZ		5 /* return kernel idea of PR_SLOWHZ */
#define TCPCTL_BADDYNAMIC	6 /* return bad dynamic port bitmap */
#define	TCPCTL_RECVSPACE	7 /* receive buffer space */
#define	TCPCTL_SENDSPACE	8 /* send buffer space */
#define	TCPCTL_IDENT		9 /* get connection owner */
#define	TCPCTL_SACK	       10 /* selective acknowledgement, rfc 2018 */
#define TCPCTL_MSSDFLT	       11 /* Default maximum segment size */
#define	TCPCTL_RSTPPSLIMIT     12 /* RST pps limit */
#define	TCPCTL_ACK_ON_PUSH     13 /* ACK immediately on PUSH */
#define	TCPCTL_ECN	       14 /* RFC3168 ECN */
#define	TCPCTL_MAXID	       15

#define	TCPCTL_NAMES { \
	{ 0, 0 }, \
	{ "rfc1323",	CTLTYPE_INT }, \
	{ "keepinittime",	CTLTYPE_INT }, \
	{ "keepidle",	CTLTYPE_INT }, \
	{ "keepintvl",	CTLTYPE_INT }, \
	{ "slowhz",	CTLTYPE_INT }, \
	{ "baddynamic", CTLTYPE_STRUCT }, \
	{ "recvspace",	CTLTYPE_INT }, \
	{ "sendspace",	CTLTYPE_INT }, \
	{ "ident", 	CTLTYPE_STRUCT }, \
	{ "sack",	CTLTYPE_INT }, \
	{ "mssdflt",	CTLTYPE_INT }, \
	{ "rstppslimit",	CTLTYPE_INT }, \
	{ "ackonpush",	CTLTYPE_INT }, \
	{ "ecn", 	CTLTYPE_INT }, \
}

struct tcp_ident_mapping {
	struct sockaddr_storage faddr, laddr;
	int euid, ruid;
};

#ifdef _KERNEL
extern	struct inpcbtable tcbtable;	/* head of queue of active tcpcb's */
extern	struct tcpstat tcpstat;	/* tcp statistics */
extern	u_int32_t tcp_now;		/* for RFC 1323 timestamps */
extern	int tcp_do_rfc1323;	/* enabled/disabled? */
extern	int tcp_mssdflt;	/* default maximum segment size */
extern	int tcp_ack_on_push;	/* ACK immediately on PUSH */
#ifdef TCP_SACK
extern	int tcp_do_sack;	/* SACK enabled/disabled */
extern	struct pool sackhl_pool;
#endif
extern	int tcp_do_ecn;		/* RFC3168 ECN enabled/disabled? */

int	 tcp_attach(struct socket *);
void	 tcp_canceltimers(struct tcpcb *);
struct tcpcb *
	 tcp_close(struct tcpcb *);
#if defined(INET6) && !defined(TCP6)
void	 tcp6_ctlinput(int, struct sockaddr *, void *);
#endif
void	 *tcp_ctlinput(int, struct sockaddr *, void *);
int	 tcp_ctloutput(int, struct socket *, int, int, struct mbuf **);
struct tcpcb *
	 tcp_disconnect(struct tcpcb *);
struct tcpcb *
	 tcp_drop(struct tcpcb *, int);
void	 tcp_dooptions(struct tcpcb *, u_char *, int, struct tcphdr *,
		int *, u_int32_t *, u_int32_t *);
void	 tcp_drain(void);
void	 tcp_init(void);
#if defined(INET6) && !defined(TCP6)
int	 tcp6_input(struct mbuf **, int *, int);
#endif
void	 tcp_input(struct mbuf *, ...);
int	 tcp_mss(struct tcpcb *, int);
void	 tcp_mss_update(struct tcpcb *);
void	 tcp_mtudisc(struct inpcb *, int);
void	 tcp_mtudisc_increase(struct inpcb *, int);
#ifdef INET6
void	tcp6_mtudisc(struct inpcb *, int);
void	tcp6_mtudisc_callback(struct in6_addr *);
#endif
struct tcpcb *
	 tcp_newtcpcb(struct inpcb *);
void	 tcp_notify(struct inpcb *, int);
int	 tcp_output(struct tcpcb *);
void	 tcp_pulloutofband(struct socket *, u_int, struct mbuf *, int);
void	 tcp_quench(struct inpcb *, int);
int	 tcp_reass(struct tcpcb *, struct tcphdr *, struct mbuf *, int *);
void	 tcp_rscale(struct tcpcb *, u_long);
void	 tcp_respond(struct tcpcb *, caddr_t, struct mbuf *, tcp_seq,
		tcp_seq, int);
void	 tcp_setpersist(struct tcpcb *);
void	 tcp_slowtimo(void);
struct mbuf *
	 tcp_template(struct tcpcb *);
void	 tcp_trace(int, int, struct tcpcb *, caddr_t, int, int);
struct tcpcb *
	 tcp_usrclosed(struct tcpcb *);
int	 tcp_sysctl(int *, u_int, void *, size_t *, void *, size_t);
#if defined(INET6) && !defined(TCP6)
int	 tcp6_usrreq(struct socket *,
	    int, struct mbuf *, struct mbuf *, struct mbuf *, struct proc *);
#endif
int	 tcp_usrreq(struct socket *,
	    int, struct mbuf *, struct mbuf *, struct mbuf *);
void	 tcp_xmit_timer(struct tcpcb *, int);
void	 tcpdropoldhalfopen(struct tcpcb *, u_int16_t);
#ifdef TCP_SACK
int	 tcp_sack_option(struct tcpcb *,struct tcphdr *,u_char *,int);
void	 tcp_update_sack_list(struct tcpcb *tp);
void	 tcp_del_sackholes(struct tcpcb *, struct tcphdr *);
void	 tcp_clean_sackreport(struct tcpcb *tp);
void	 tcp_sack_adjust(struct tcpcb *tp);
struct sackhole *
	 tcp_sack_output(struct tcpcb *tp);
int	 tcp_sack_partialack(struct tcpcb *, struct tcphdr *);
#ifdef DEBUG
void	 tcp_print_holes(struct tcpcb *tp);
#endif
#endif /* TCP_SACK */
#if defined(TCP_SACK)
int	 tcp_newreno(struct tcpcb *, struct tcphdr *);
u_long	 tcp_seq_subtract(u_long, u_long );
#endif /* TCP_SACK */
#ifdef TCP_SIGNATURE
int	tcp_signature_apply(caddr_t, caddr_t, unsigned int);
#endif /* TCP_SIGNATURE */
void	tcp_rndiss_init(void);
tcp_seq	tcp_rndiss_next(void);
u_int16_t
	tcp_rndiss_encrypt(u_int16_t);
#endif /* _KERNEL */
#endif /* _NETINET_TCP_VAR_H_ */
