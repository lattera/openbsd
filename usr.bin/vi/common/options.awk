#	$OpenBSD: src/usr.bin/vi/common/options.awk,v 1.3 2001/01/29 01:58:31 niklas Exp $

#	@(#)options.awk	10.1 (Berkeley) 6/8/95
 
/^\/\* O_[0-9A-Z_]*/ {
	printf("#define %s %d\n", $2, cnt++);
	next;
}
END {
	printf("#define O_OPTIONCOUNT %d\n", cnt);
}
