#objdump: -d
#name: c54x local labels

.*: +file format .*c54x.*

Disassembly of section .text:

00000000 <label1>:
   0:	1000.*
   1:	0800.*
   2:	f843.*
   3:	0007.*
   4:	1000.*
   5:	f073.*
   6:	0008.*

00000007 <\$1.*>:
   7:	1000.*

00000008 <\$2.*>:
   8:	0000.*
   9:	f843.*
   a:	000c.*
   b:	8000.*

0000000c <\$1.*>:
   c:	f495.*
   d:	f495.*

0000000e <lab.*>:
   e:	f000.*
   f:	0001.*
  10:	f073.*
  11:	000e.*

00000012 <lab.*>:
  12:	f845.*
  13:	0012.*

00000014 <lab.*>:
  14:	f000.*
  15:	0003.*
  16:	f073.*
  17:	0014.*

00000018 <lab.*>:
  18:	f000.*
  19:	0004.*
  1a:	f073.*
  1b:	0018.*

0000001c <after_macro>:
  1c:	f073.*
  1d:	0014.*
Disassembly of section new_sect:

00000000 <new_section>:
   0:	f495.*

00000001 <lab.7>:
   1:	f000.*
   2:	0005.*
   3:	f495.*
   4:	f495.*
   5:	f073.*
   6:	0001.*

00000007 <lab.8>:
   7:	f000.*
   8:	0006.*
   9:	f495.*
   a:	f495.*
   b:	f073.*
   c:	0007.*
