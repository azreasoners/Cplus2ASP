for ((i=1; i<=$3; i++)); do
	coala -i no $1 -s $2 -o raw -c t=$i
done
