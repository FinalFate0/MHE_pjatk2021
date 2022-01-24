import subprocess
import re
import numpy as np
import pandas as pd

statistics = {
    "tempdivide":[],
    "tempdividelog":[],
    "temppow":[]

}
mean_statistics = {
    "tempdivide":[],
    "tempdividelog":[],
    "temppow":[]
}

parameter_multiplier = [
    0.00000001,
    0.00000002,
    0.00000003,
    0.00000004,
    0.00000005,
    0.00000006,
    0.00000007,
    0.00000008,
    0.00000009,
    0.0000001,
    0.0000002,
    0.0000003,
    0.0000004,
    0.0000005,
    0.0000006,
    0.0000007,
    0.0000008,
    0.0000009,
    0.000001,
    0.000002,
    0.000003,
    0.000004,
    0.000005,
    0.000006,
    0.000007,
    0.000008,
    0.000009,
    0.00001,
    0.00002,
    0.00003,
    0.00004,
    0.00005,
    0.00006,
    0.00007,
    0.00008,
    0.00009,
    0.0001,
    0.0002,
    0.0003,
    0.0004,
    0.0005,
    0.0006,
    0.0007,
    0.0008,
    0.0009,
    0.001,
    0.002,
    0.003,
    0.004,
    0.005,
    0.006,
    0.007,
    0.008,
    0.009,
    0.01,
    0.02,
    0.03,
    0.04,
    0.05,
    0.06,
    0.07,
    0.08,
    0.09,
    0.1,
    0.2,
    0.3,
    0.4,
    0.5,
    0.6,
    0.7,
    0.8,
    0.9,
    1.0
    
]


for method_name in statistics:
    for pm in parameter_multiplier:
        for repeat in range(0,15):
            
            if method_name == "tempdivide":
                parameter_base = 1000
            elif method_name == "tempdividelog":
                parameter_base = 1000
            elif method_name == "temppow":
                parameter_base = 0.99
                
            
            output_name = "output.txt"
            cmndName = "sa --output " + str(output_name) + " --temperature " + method_name + " --temperature_parameter " + str(pm*parameter_base) + " --iterations 50000"
            print(cmndName)
            subprocess.run(['cmd.exe', '/c', cmndName])
            result = open(output_name)
                    
            time_line = ""
            cost_line = ""
            for line in result:
                if "result:" in line:
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


            statistics[method_name].append([pm, float(cost[0]), int(microseconds_total)])
            
        cost_results = []
        time_results = []
        for element in statistics[method_name]: 
            cost_results.append(element[1])
            time_results.append(element[2])
            
        mean_cost = np.mean(cost_results)
        mean_time = np.mean(time_results)
        mean_statistics[method_name].append([pm, float(mean_cost), float(mean_time)])
                

print(mean_statistics)

for method in mean_statistics:
    column_names = ['parameter multiplier', 'mean result', 'mean time']
    dataframe = pd.DataFrame(mean_statistics[method], columns=column_names)

    dataframe.to_csv(method + ".csv")
    
# biggest_results = []

# for method in mean_statistics:
#     for row in mean_statistics[method]:
#         if row[0] == parameter_multiplier[-1]:
#             biggest_results.append(row)

# dataframe = pd.DataFrame(biggest_results, columns=column_names)

# dataframe.to_csv("biggest_results.csv")



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