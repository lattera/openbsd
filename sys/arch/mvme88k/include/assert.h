/*	$OpenBSD: src/sys/arch/mvme88k/include/Attic/assert.h,v 1.4.6.1 2001/04/18 16:11:12 niklas Exp $ */
#ifndef __MACHINE_ASSERT_H__
#define __MACHINE_ASSERT_H__
#ifndef assert
#define assert(x) \
({\
	if (!(x)) {\
		printf("assertion failure \"%s\" line %d file %s\n", \
		#x, __LINE__, __FILE__); \
		panic("assertion"); \
	} \
})
#endif /* assert */
#endif __MACHINE_ASSERT_H__

