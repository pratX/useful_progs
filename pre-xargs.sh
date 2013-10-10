#! /bin/bash
file=$1
db_file=$2
num_lines=`wc -l $1 | sed 's/ .*//'`
lines_per_loop=$3
echo "$lines_per_loop  $num_lines"
#line_no=1
num_std_loops=`expr $num_lines / $lines_per_loop`
echo $num_std_loops
lines_last_loop=`expr $num_lines % $lines_per_loop`
echo $lines_last_loop
while [ $num_std_loops -gt 0 ]
do
  ./pre-xargs $file $db_file $lines_per_loop | xargs $4 
  num_std_loops=$(( num_std_loops - 1  ))    
done
./pre-xargs $file $db_file $lines_per_loop | xargs $4
