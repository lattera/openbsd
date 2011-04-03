/*
 * Public Domain 2003 Dale Rahn
 *
 * $OpenBSD: src/regress/libexec/ld.so/dlopen/libab/ab.h,v 1.2 2011/04/03 22:29:50 drahn Exp $
 */

class BB {
public:
        BB(const char *);
        ~BB();
private:
	const char *_name;
};

