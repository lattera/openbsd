/* $OpenBSD: src/sys/arch/amiga/dev/Attic/ite_blank.h,v 1.1.2.1 2002/03/28 10:06:14 niklas Exp $ */
void ite_restart_blanker(struct ite_softc *);
void ite_reset_blanker(struct ite_softc *);
void ite_disable_blanker(struct ite_softc *);
void ite_enable_blanker(struct ite_softc *);

extern int blank_time;
