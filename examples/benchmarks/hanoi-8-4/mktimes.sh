#!/bin/sh
cat $@ | grep '\(^[#]\)\|\(Time\)\|\(Total time\)\|\(Solution time\)' | sed 's/(.*//g' | sed 's/^.*:\s*//g' | sed 's/seconds.*//g'
