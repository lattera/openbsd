
/* $OpenBSD: src/sys/arch/i386/stand/libsa/exec_i386.c,v 1.2 1997/03/31 05:52:25 weingart Exp $ */

#include <sys/param.h>
#include <sys/exec.h>
#include <sys/reboot.h>
#include <libsa.h>


void
machdep_start(startaddr, howto, loadaddr, ssym, esym)
	char *startaddr, *loadaddr, *ssym, *esym;
	int howto;
{
	static int argv[9];
	struct exec *x;


#ifdef DEBUG
	x = (void *)loadaddr;
	printf("exec {\n");
	printf("  a_midmag = %lx\n", x->a_midmag);
	printf("  a_text = %lx\n", x->a_text);
	printf("  a_data = %lx\n", x->a_data);
	printf("  a_bss = %lx\n", x->a_bss);
	printf("  a_syms = %lx\n", x->a_syms);
	printf("  a_entry = %lx\n", x->a_entry);
	printf("  a_trsize = %lx\n", x->a_trsize);
	printf("  a_drsize = %lx\n", x->a_drsize);
	printf("}\n");

	getchar();
#endif

	(int)startaddr &= 0xffffff;

	/*
	 *  We now pass the various bootstrap parameters to the loaded
	 *  image via the argument list
	 *
	 *  arg0 = 8 (magic)
	 *  arg1 = boot flags
	 *  arg2 = boot device
	 *  arg3 = Cylinder offset (XXX - used to be ssym)
	 *  arg4 = end of symbol table (0 if not loaded)
	 *  arg5 = transfer address from image
	 *  arg6 = transfer address for next image pointer
	 *  arg7 = conventional memory size (640)
	 *  arg8 = extended memory size (8196)
	 */
	argv[0] = 8;
	argv[1] = howto;
	argv[2] = bootdev;		/* Boot device */
	argv[3] = 0;			/* Cyl offset */
	argv[4] = 0;
	argv[5] = (int)startaddr;
	argv[6] = 0;
	argv[7] = memsize(0);
	argv[8] = memsize(1);

#ifdef DEBUG
	{ int i;
		for(i = 0; i <= argv[0]; i++)
			printf("argv[%d] = %x\n", i, argv[i]);

		getchar();
	}
#endif

	/****************************************************************/
	/* copy that first page and overwrite any BIOS variables        */
	/****************************************************************/
	printf("entry point at 0x%x\n", (int)startaddr);
	startprog(startaddr, argv);
}

