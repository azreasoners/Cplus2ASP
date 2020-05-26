#!/bin/sh
cat $@ | grep -i '\(Time\)\|\(Prepare\)\|\(Prepro\)' \
	| sed 's/\s*Time[^:]*:\s*//g' \
	| sed 'N;N;s/\s*\n\s*Pre[^:]*:\s*/ /g'

#	| sed 'N;s/\s*\n\s*Prepare[^:]*:\s*/ /g'\
#	| sed 'N;s/\s*\n\s*Prepro[^:]*:\s*/ /g'

