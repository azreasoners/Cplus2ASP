cplus2asp ferryman-120-4-benchmark.cp --stats query=0 --none-alias -transout=trans.out -preout=f2lp.out -soout=ferryman-120-4.out -soopt="--iheuristic=keep" &
sleep 1
./mem_tracker.sh iclingo > ferryman-120-4.mem &
