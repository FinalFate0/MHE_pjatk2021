set term png size 1280, 960
set output "iterationslog.png"
set title "Iterations to result"
set xlabel "Iterations"
set ylabel "Result"
set logscale y 10
set datafile separator ","
plot 'iterations.csv' u 1:2 w lines
