#!/bin/sh
cat $@ | grep '\(^[#]\)\|\(Time\)\|\(Total time\)\|\(Solution time\)' | sed 's/^[^:]*:\s*//g' | sed 's/(g:\s*/ /g' | sed 's/,\s*p: / /g' | sed 's/,\s*s: / /g' | sed 's/)//g'

