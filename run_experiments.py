import subprocess
import re
import numpy as np
import pandas as pd

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
    "3x3",
    "3x4",
    "4x4",
    "5x4",
    "5x5",
    "6x6",
    "8x8",
    "9x9",
    "10x10",
    # "15x15"
    
]


for method_name in statistics:
    for problem in problem_size:
        for repeat in range(0,5):
            output_name = "output.txt"
            cmndName = "nonogram --output " + str(output_name) + " --input " + str(problem) +  ".txt --method " + method_name + " --iterations 25000 --tabusize 64"
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
            microseconds = 0
            
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
                    microseconds = int(unit_value)

            microseconds_total = (hours*3600000000)+(minutes*60000000)+(seconds*1000000)+(milliseconds*1000)+microseconds


            statistics[method_name].append([problem, float(cost[0]), int(microseconds_total)])
            
        cost_results = []
        time_results = []
        for element in statistics[method_name]: 
            cost_results.append(element[1])
            time_results.append(element[2])
            
        mean_cost = np.mean(cost_results)
        mean_time = np.mean(time_results)
        mean_statistics[method_name].append([problem, float(mean_cost), float(mean_time)])
                

print(mean_statistics)

for method in mean_statistics:
    column_names = ['problem', 'mean cost', 'mean time']
    dataframe = pd.DataFrame(mean_statistics[method], columns=column_names)

    dataframe.to_csv(method + ".csv")
    
biggest_results = []

for method in mean_statistics:
    for row in mean_statistics[method]:
        if row[0] == problem_size[-1]:
            biggest_results.append(row)

dataframe = pd.DataFrame(biggest_results, columns=column_names)

dataframe.to_csv("biggest_results.csv")



with open("gnuplot time.plt", "w") as gnuplotfile:
    gnuplotfile.write("set term png\n")
    gnuplotfile.write("set output \"time_to_size.png\"\n") 
    gnuplotfile.write("set title \"Time to problem size\"\n") 
    gnuplotfile.write("set xlabel \"Problem size\"\n") 
    gnuplotfile.write("set ylabel \"Time\"\n") 
    gnuplotfile.write("set datafile separator \",\"\n")
    gnuplotfile.write("plot ")
    for method_name in mean_statistics:
        # print(method_name)
        gnuplotfile.write("'" + method_name + ".csv' u 1:4 w lines, ") 

    gnuplotfile.write("\n") 
    
with open("gnuplot biggest.plt", "w") as gnuplotfile2:
    gnuplotfile2.write("set term png\n")
    gnuplotfile2.write("set output \"biggest_problem.png\"\n") 
    gnuplotfile2.write("set title \"Cost to time for the biggest problem\"\n") 
    gnuplotfile2.write("set xlabel \"Time\"\n") 
    gnuplotfile2.write("set ylabel \"Cost\"\n") 
    gnuplotfile2.write("set datafile separator \",\"\n")
    gnuplotfile2.write("plot ")
    # print("biggest problem")
    gnuplotfile2.write("'biggest_results.csv' u 4:3 w lines, ") 

    gnuplotfile2.write("\n") 
    
with open("gnuplot cost.plt", "w") as gnuplotfile3:
    gnuplotfile3.write("set term png\n")
    gnuplotfile3.write("set output \"size_to_cost.png\"\n") 
    gnuplotfile3.write("set title \"Cost to problem size\"\n") 
    gnuplotfile3.write("set xlabel \"Problem size\"\n") 
    gnuplotfile3.write("set ylabel \"Cost\"\n") 
    gnuplotfile3.write("set datafile separator \",\"\n")
    gnuplotfile3.write("plot ")
    for method_name in mean_statistics:
        # print(method_name)
        gnuplotfile3.write("'" + method_name + ".csv' u 1:3 w lines, ") 

    gnuplotfile3.write("\n") 

subprocess.run(['cmd.exe', '/c', "gnuplot time.plt"])
subprocess.run(['cmd.exe', '/c', "gnuplot biggest.plt"])
subprocess.run(['cmd.exe', '/c', "gnuplot cost.plt"])