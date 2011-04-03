/*
 * Public Domain 2003 Dale Rahn
 *
 * $OpenBSD: src/regress/libexec/ld.so/dlopen/libac/ac.h,v 1.2 2011/04/03 22:29:50 drahn Exp $
 */

class AC {
public:
        AC(const char *);
        ~AC();
private:
	const char *_name;
};

