set term png size 1280, 960
set output "pm to result.png"
set title "Result to parameter multiplier"
set xlabel "Parameter multiplier"
set ylabel "Result"
set datafile separator ","
set logscale x 10
plot 'tempdivide.csv' u 2:3 w lines, 'tempdividelog.csv' u 2:3 w lines, 'temppow.csv' u 2:3 w lines, 
