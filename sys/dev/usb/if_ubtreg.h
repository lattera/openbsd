/*	$OpenBSD: src/sys/dev/usb/Attic/if_ubtreg.h,v 1.1 2005/01/14 12:21:02 grange Exp $	*/

/*
 * ng_ubt_var.h
 *
 * Copyright (c) 2001-2002 Maksim Yevmenkin <m_evmenkin@yahoo.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: ng_ubt_var.h,v 1.2 2003/03/22 23:44:36 max Exp $
 * $FreeBSD: src/sys/netgraph/bluetooth/drivers/ubt/ng_ubt_var.h,v 1.4 2004/10/12 23:33:46 emax Exp $
 */

#ifndef _NG_UBT_VAR_H_
#define _NG_UBT_VAR_H_

/**************************************************************************
 **************************************************************************
 **     Netgraph node hook name, type name and type cookie and commands 
 **************************************************************************  
 **************************************************************************/

#define NG_UBT_NODE_TYPE	"ubt"
#define NG_UBT_HOOK		"hook"

#define NGM_UBT_COOKIE		1021837971

/* Debug levels */
#define NG_UBT_ALERT_LEVEL	1
#define NG_UBT_ERR_LEVEL	2
#define NG_UBT_WARN_LEVEL	3
#define NG_UBT_INFO_LEVEL	4

/**************************************************************************
 **************************************************************************
 **                    UBT node command/event parameters
 **************************************************************************
 **************************************************************************/

#define NGM_UBT_NODE_SET_DEBUG	1		/* set debug level */
#define NGM_UBT_NODE_GET_DEBUG	2		/* get debug level */
typedef u_int16_t		ng_ubt_node_debug_ep; 

#define NGM_UBT_NODE_SET_QLEN	3		/* set queue length */
#define NGM_UBT_NODE_GET_QLEN	4		/* get queue length */ 
typedef struct {
	int32_t		queue;			/* queue index */
#define	NGM_UBT_NODE_QUEUE_CMD	1		/* commands */
#define	NGM_UBT_NODE_QUEUE_ACL	2		/* ACL data */
#define	NGM_UBT_NODE_QUEUE_SCO	3		/* SCO data */

	int32_t		qlen;			/* queue length */
} ng_ubt_node_qlen_ep;

#define NGM_UBT_NODE_GET_STAT	5		/* get statistic */
typedef struct {
	u_int32_t	pckts_recv;		/* # of packets received */
	u_int32_t	bytes_recv;		/* # of bytes received */
	u_int32_t	pckts_sent;		/* # of packets sent */
	u_int32_t	bytes_sent;		/* # of bytes sent */
	u_int32_t	oerrors;		/* # of output errors */
	u_int32_t	ierrors;		/* # of input errors */
} ng_ubt_node_stat_ep;

#define NGM_UBT_NODE_RESET_STAT	6		/* reset statistic */

#define NGM_UBT_NODE_DEV_NODES	7		/* on/off device interface */
typedef u_int16_t	ng_ubt_node_dev_nodes_ep;

/* pullup wrapper */
#define NG_UBT_M_PULLUP(m, s) \
	do { \
		if ((m)->m_len < (s)) \
			(m) = m_pullup((m), (s)); \
		if ((m) == NULL) \
			NG_UBT_ALERT("%s: %s - m_pullup(%d) failed\n", \
				__func__, USBDEVNAME(sc->sc_dev), (s)); \
	} while (0)

/* Debug printf's */
#define NG_UBT_ALERT	if (sc->sc_debug >= NG_UBT_ALERT_LEVEL) printf
#define NG_UBT_ERR	if (sc->sc_debug >= NG_UBT_ERR_LEVEL)   printf
#define NG_UBT_WARN	if (sc->sc_debug >= NG_UBT_WARN_LEVEL)  printf
#define NG_UBT_INFO	if (sc->sc_debug >= NG_UBT_INFO_LEVEL)  printf

/* Bluetooth USB control request type */
#define UBT_HCI_REQUEST		0x20
#define UBT_DEFAULT_QLEN	12

/* USB device softc structure */
struct ubt_softc {
	USBBASEDEVICE		 sc_dev;	/* pointer back to USB device */

	/* State */
	ng_ubt_node_debug_ep	 sc_debug;	/* debug level */
	u_int32_t		 sc_flags;	/* device flags */
#define UBT_NEED_FRAME_TYPE	(1 << 0)	/* device required frame type */
#define UBT_HAVE_FRAME_TYPE UBT_NEED_FRAME_TYPE
#define UBT_CMD_XMIT		(1 << 1)	/* CMD xmit in progress */
#define UBT_ACL_XMIT		(1 << 2)	/* ACL xmit in progress */
#define UBT_SCO_XMIT		(1 << 3)	/* SCO xmit in progress */
#define UBT_EVT_RECV		(1 << 4)	/* EVN recv in progress */
#define UBT_ACL_RECV		(1 << 5)	/* ACL recv in progress */
#define UBT_SCO_RECV		(1 << 6)	/* SCO recv in progress */
#define UBT_CTRL_DEV		(1 << 7)	/* ctrl device is open */
#define UBT_INTR_DEV		(1 << 8)	/* intr device is open */
#define UBT_BULK_DEV		(1 << 9)	/* bulk device is open */
#define UBT_ANY_DEV		(UBT_CTRL_DEV|UBT_INTR_DEV|UBT_BULK_DEV)

	ng_ubt_node_stat_ep	 sc_stat;	/* statistic */
#define NG_UBT_STAT_PCKTS_SENT(s)	(s).pckts_sent ++
#define NG_UBT_STAT_BYTES_SENT(s, n)	(s).bytes_sent += (n)
#define NG_UBT_STAT_PCKTS_RECV(s)	(s).pckts_recv ++
#define NG_UBT_STAT_BYTES_RECV(s, n)	(s).bytes_recv += (n)
#define NG_UBT_STAT_OERROR(s)		(s).oerrors ++
#define NG_UBT_STAT_IERROR(s)		(s).ierrors ++
#define NG_UBT_STAT_RESET(s)		bzero(&(s), sizeof((s)))

	/* USB device specific */
	usbd_device_handle	 sc_udev;	/* USB device handle */

	usbd_interface_handle	 sc_iface0;	/* USB interface 0 */
	usbd_interface_handle	 sc_iface1;	/* USB interface 1 */

	/* Interrupt pipe (HCI events) */
	int			 sc_intr_ep;	/* interrupt endpoint */
	usbd_pipe_handle	 sc_intr_pipe;	/* interrupt pipe handle */
	usbd_xfer_handle	 sc_intr_xfer;	/* intr xfer */
	struct mbuf		*sc_intr_mbuf;	/* interrupt mbuf */
	void			*sc_intr_buffer; /* interrupt buffer */
#define UBT_INTR_BUFFER_SIZE MCLBYTES

	/* Control pipe (HCI commands) */
	usbd_xfer_handle	 sc_ctrl_xfer;	/* control xfer handle */
	void			*sc_ctrl_buffer; /* control buffer */
	struct ng_bt_mbufq	 sc_cmdq;	/* HCI command queue */
#define UBT_CTRL_BUFFER_SIZE \
		(sizeof(ng_hci_cmd_pkt_t) + NG_HCI_CMD_PKT_SIZE)

	/* Bulk in pipe (ACL data) */
	int			 sc_bulk_in_ep;	/* bulk-in enpoint */
	usbd_pipe_handle	 sc_bulk_in_pipe; /* bulk-in pipe */
	usbd_xfer_handle	 sc_bulk_in_xfer; /* bulk-in xfer */
	struct mbuf		*sc_bulk_in_mbuf; /* bulk-in mbuf */
	void			*sc_bulk_in_buffer; /* bulk-in buffer */

	/* Bulk out pipe (ACL data) */
	int			 sc_bulk_out_ep; /* bulk-out endpoint */
	usbd_pipe_handle	 sc_bulk_out_pipe; /* bulk-out pipe */
	usbd_xfer_handle	 sc_bulk_out_xfer; /* bulk-out xfer */
	void			*sc_bulk_out_buffer; /* bulk-out buffer */
	struct ng_bt_mbufq	 sc_aclq;	/* ACL data queue */
#define UBT_BULK_BUFFER_SIZE \
		MCLBYTES /* XXX should be big enough to hold one frame */

	/* Isoc. in pipe (SCO data) */
	int			 sc_isoc_in_ep; /* isoc-in endpoint */
	usbd_pipe_handle	 sc_isoc_in_pipe; /* isoc-in pipe */
	usbd_xfer_handle	 sc_isoc_in_xfer; /* isoc-in xfer */
	void			*sc_isoc_in_buffer; /* isoc-in buffer */
	u_int16_t		*sc_isoc_in_frlen; /* isoc-in. frame length */

	/* Isoc. out pipe (ACL data) */
	int			 sc_isoc_out_ep; /* isoc-out endpoint */
	usbd_pipe_handle	 sc_isoc_out_pipe; /* isoc-out pipe */
	usbd_xfer_handle	 sc_isoc_out_xfer; /* isoc-out xfer */
	void			*sc_isoc_out_buffer; /* isoc-in buffer */
	u_int16_t		*sc_isoc_out_frlen; /* isoc-out. frame length */
	struct ng_bt_mbufq	 sc_scoq;	/* SCO data queue */

	int			 sc_isoc_size; /* max. size of isoc. packet */
	u_int32_t		 sc_isoc_nframes; /* num. isoc. frames */
#define UBT_ISOC_BUFFER_SIZE \
		(sizeof(ng_hci_scodata_pkt_t) + NG_HCI_SCO_PKT_SIZE)

#if 0
	/* Netgraph specific */
	node_p			 sc_node;	/* pointer back to node */
	hook_p			 sc_hook;	/* upstream hook */
#endif

	struct ifnet		sc_if;
};
typedef struct ubt_softc	ubt_softc_t;
typedef struct ubt_softc *	ubt_softc_p;

#endif /* ndef _NG_UBT_VAR_H_ */
