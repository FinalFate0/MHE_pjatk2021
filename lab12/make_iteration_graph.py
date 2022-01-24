import subprocess

with open("iterations.plt", "w") as gnuplotfile:
    gnuplotfile.write("set term png size 1280, 960\n")
    gnuplotfile.write("set output \"iterationslog.png\"\n") 
    gnuplotfile.write("set title \"Iterations to result\"\n") 
    gnuplotfile.write("set xlabel \"Iterations\"\n") 
    gnuplotfile.write("set ylabel \"Result\"\n") 
    gnuplotfile.write("set logscale y 10\n")
    gnuplotfile.write("set datafile separator \",\"\n")
    gnuplotfile.write("plot ")

    gnuplotfile.write("'iterations.csv' u 1:2 w lines") 

    gnuplotfile.write("\n") 
    
    gnuplotfile.close()
    subprocess.run(['cmd.exe', '/c', "iterations.plt"])
