import subprocess

mean_statistics = {
    "tempdivide":[],
    "tempdividelog":[],
    "temppow":[]
}

with open("gnuplot time.plt", "w") as gnuplotfile:
    gnuplotfile.write("set term png size 1280, 960\n")
    gnuplotfile.write("set output \"time_to_pm.png\"\n") 
    gnuplotfile.write("set title \"Time to parameter multiplier\"\n") 
    gnuplotfile.write("set xlabel \"parameter multiplier\"\n") 
    gnuplotfile.write("set ylabel \"Time\"\n") 
    gnuplotfile.write("set datafile separator \",\"\n")
    gnuplotfile.write("set logscale x 10\n")
    gnuplotfile.write("plot ")
    for method_name in mean_statistics:
        # print(method_name)
        gnuplotfile.write("'" + method_name + ".csv' u 2:4 w lines, ") 

    gnuplotfile.write("\n") 
    gnuplotfile.close()
    
    
with open("gnuplot result.plt", "w") as gnuplotfile2:
    gnuplotfile2.write("set term png size 1280, 960\n")
    gnuplotfile2.write("set output \"pm to result.png\"\n") 
    gnuplotfile2.write("set title \"Result to parameter multiplier\"\n") 
    gnuplotfile2.write("set xlabel \"Parameter multiplier\"\n") 
    gnuplotfile2.write("set ylabel \"Result\"\n") 
    gnuplotfile2.write("set datafile separator \",\"\n")
    gnuplotfile2.write("set logscale x 10\n")
    gnuplotfile2.write("plot ")
    for method_name in mean_statistics:
        # print(method_name)
        gnuplotfile2.write("'" + method_name + ".csv' u 2:3 w lines, ") 

    gnuplotfile2.write("\n")
    gnuplotfile2.close()

subprocess.run(['cmd.exe', '/c', "gnuplot time.plt"])
subprocess.run(['cmd.exe', '/c', "gnuplot result.plt"])