#!./perl

# $RCSfile: cond.t,v $$Revision: 1.7 $$Date: 2003/12/03 03:02:47 $

# make sure conditional operators work

print "1..4\n";

$x = '0';

$x eq $x && (print "ok 1\n");
$x ne $x && (print "not ok 1\n");
$x eq $x || (print "not ok 2\n");
$x ne $x || (print "ok 2\n");

$x == $x && (print "ok 3\n");
$x != $x && (print "not ok 3\n");
$x == $x || (print "not ok 4\n");
$x != $x || (print "ok 4\n");
