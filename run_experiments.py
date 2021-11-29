import subprocess
import re
import numpy as np
import pandas as pd
from time import gmtime, strftime
# import PyGnuplot as gp

statistics = {
    # "bruteforce":[],
    "hillclimb":[],
    "hillclimb_stch":[],
    "tabu":[]

}
mean_statistics = {
    # "bruteforce":[],
    "hillclimb":[],
    "hillclimb_stch":[],
    "tabu":[]

}

problem_size = [
    # "3x3",
    # "3x4",
    "4x4",
    "5x4",
    "5x5",
    "6x6",
    "8x8",
    # "9x9",
    # "10x10",
    # "15x15"
    
]


for method_name in statistics:
    for problem in problem_size:
        for repeat in range(0,1):
            output_name = "output.txt"
            cmndName = "nonogram --output " + str(output_name) + " --input " + str(problem) +  ".txt --method " + method_name + " --iterations 1000000 --tabusize 20000"
            print(cmndName)
            subprocess.run(['cmd.exe', '/c', cmndName])
            result = open(output_name)
                    
            time_line = ""
            cost_line = ""
            for line in result:
                if "cost:" in line:
                    cost_line = line
                elif "time elapsed:" in line:
                    time_line = line
                        
            cost = re.findall(re.compile('[0-9]+.+[0-9]|[0-9]'), cost_line)
            time = re.findall(re.compile('[0-9]+[a-z]*[a-z]'), time_line)
            hours = 0
            minutes = 0
            seconds = 0
            milliseconds = 0
            microsecond = 0
            
            for unit in time:
                unit_name = re.sub('\d', '', unit)
                unit_value = re.sub('\D', '', unit)
                if "h" == unit_name:
                    hours = int(unit_value)
                elif "m" == unit_name:
                    minutes = int(unit_value)
                elif "s" == unit_name:
                    seconds = int(unit_value)
                elif "ms" == unit_name:
                    milliseconds = int(unit_value)
                elif "us" == unit_name:
                    microsecond = int(unit_value)


            seconds_total = (hours*3600)+(minutes*60)+seconds
            if milliseconds >= 500:
                seconds_total += 1

            statistics[method_name].append([problem, float(cost[0]), int(seconds_total)])
            
        cost_results = []
        time_results = []
        for element in statistics[method_name]: 
            cost_results.append(element[1])
            time_results.append(element[2])
            
        mean_cost = np.mean(cost_results)
        mean_time = np.mean(time_results)
        mean_statistics[method_name].append([problem, float(mean_cost), strftime('%H:%M:%S', gmtime(int(mean_time)))])
                

print(mean_statistics)

for method in mean_statistics:
    column_names = ['problem', 'mean cost', 'mean time']
    dataframe = pd.DataFrame(mean_statistics[method], columns=column_names)

    dataframe.to_csv(method + ".csv")
    
    
with open("gnuplot result.plt", "w") as gnuplotfile:
    gnuplotfile.write("set term png\n")
    gnuplotfile.write("set output \"result.png\"\n") 
    gnuplotfile.write("set datafile separator \",\"\n")
    gnuplotfile.write("set ydata time\n")
    gnuplotfile.write("set timefmt \"%H:%M:%S\"\n")
    gnuplotfile.write("set format y \"%H:%M:%S\"\n")
    gnuplotfile.write("plot ")
    for method_name in mean_statistics:
        print(method_name)
        gnuplotfile.write("'" + method_name + ".csv' u 1:4 w lines, ") 

    gnuplotfile.write("\n") 

subprocess.run(['cmd.exe', '/c', "gnuplot result.plt"])
