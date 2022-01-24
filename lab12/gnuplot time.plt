set term png size 1280, 960
set output "time_to_pm.png"
set title "Time to parameter multiplier"
set xlabel "parameter multiplier"
set ylabel "Time"
set datafile separator ","
set logscale x 10
plot 'tempdivide.csv' u 2:4 w lines, 'tempdividelog.csv' u 2:4 w lines, 'temppow.csv' u 2:4 w lines, 
