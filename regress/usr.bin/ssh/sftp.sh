#	$OpenBSD: src/regress/usr.bin/ssh/sftp.sh,v 1.5 2013/05/17 10:28:11 dtucker Exp $
#	Placed in the Public Domain.

tid="basic sftp put/get"

BUFFERSIZE="5 1000 32000 64000"
REQUESTS="1 2 10"

for B in ${BUFFERSIZE}; do
	for R in ${REQUESTS}; do
                verbose "test $tid: buffer_size $B num_requests $R"
		rm -f ${COPY}.1 ${COPY}.2
		${SFTP} -D ${SFTPSERVER} -B $B -R $R -b /dev/stdin \
		> /dev/null 2>&1 << EOF
		version
		get $DATA ${COPY}.1
		put $DATA ${COPY}.2
EOF
		r=$?
		if [ $r -ne 0 ]; then
			fail "sftp failed with $r"
		else 
			cmp $DATA ${COPY}.1 || fail "corrupted copy after get"
			cmp $DATA ${COPY}.2 || fail "corrupted copy after put"
		fi
	done
done
rm -f ${COPY}.1 ${COPY}.2                
