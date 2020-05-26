



# Header
echo "time total_kB RSS_kB dirty_kB"

#while [ "`pgrep $1`" ]
while [ 1 ]
do
	if [ "`pgrep $1`" ]
	then
		PID=`pgrep $1`
		echo `ps -o time $PID | sed 's/TIME//'` `pmap -x $PID | grep total | sed 's/total kB //'`
	else
		echo ""
	fi
	sleep 2
done

