#	$OpenBSD: src/regress/usr.bin/ssh/scp.sh,v 1.2 2004/06/16 13:15:09 dtucker Exp $
#	Placed in the Public Domain.

tid="scp"

#set -x

DATA=/bin/ls
COPY=${OBJ}/copy
COPY2=${OBJ}/copy2
DIR=${COPY}.dd
DIR2=${COPY}.dd2

SRC=`dirname ${SCRIPT}`
cp ${SRC}/scp-ssh-wrapper.sh ${OBJ}/scp-ssh-wrapper.exe
chmod 755 ${OBJ}/scp-ssh-wrapper.exe
scpopts="-q -S ${OBJ}/scp-ssh-wrapper.exe"

scpclean() {
	rm -rf ${COPY} ${COPY2} ${DIR} ${DIR2}
	mkdir ${DIR} ${DIR2}
}

verbose "$tid: simple copy local file to remote file"
scpclean
$SCP $scpopts ${DATA} somehost:${COPY} || fail "copy failed"
cmp ${DATA} ${COPY} || fail "corrupted copy"

verbose "$tid: simple copy remote file to local file"
scpclean
$SCP $scpopts somehost:${DATA} ${COPY} || fail "copy failed"
cmp ${DATA} ${COPY} || fail "corrupted copy"

verbose "$tid: simple copy local file to remote dir"
scpclean
cp ${DATA} ${COPY}
$SCP $scpopts ${COPY} somehost:${DIR} || fail "copy failed"
cmp ${COPY} ${DIR}/copy || fail "corrupted copy"

verbose "$tid: simple copy remote file to local dir"
scpclean
cp ${DATA} ${COPY}
$SCP $scpopts somehost:${COPY} ${DIR} || fail "copy failed"
cmp ${COPY} ${DIR}/copy || fail "corrupted copy"

verbose "$tid: recursive local dir to remote dir"
scpclean
rm -rf ${DIR2}
cp ${DATA} ${DIR}/copy
$SCP $scpopts -r ${DIR} somehost:${DIR2} || fail "copy failed"
diff -rN ${DIR} ${DIR2} || fail "corrupted copy"

verbose "$tid: recursive remote dir to local dir"
scpclean
rm -rf ${DIR2}
cp ${DATA} ${DIR}/copy
$SCP $scpopts -r somehost:${DIR} ${DIR2} || fail "copy failed"
diff -rN ${DIR} ${DIR2} || fail "corrupted copy"

for i in 0 1 2 3 4; do
	verbose "$tid: disallow bad server #$i"
	SCPTESTMODE=badserver_$i
	export DIR SCPTESTMODE
	scpclean
	$SCP $scpopts somehost:${DATA} ${DIR} >/dev/null 2>/dev/null
	[ -d {$DIR}/rootpathdir ] && fail "allows dir relative to root dir"
	[ -d ${DIR}/dotpathdir ] && fail "allows dir creation in non-recursive mode"

	scpclean
	$SCP -r $scpopts somehost:${DATA} ${DIR2} >/dev/null 2>/dev/null
	[ -d ${DIR}/dotpathdir ] && fail "allows dir creation outside of subdir"
done

scpclean
rm -f ${OBJ}/scp-ssh-wrapper.exe
