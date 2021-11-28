import subprocess;
import re;
import numpy as np;

statistics = {
    "bruteforce":[],
    # "hillclimb":[],
    # "hillclimb_stch":[],
    # "tabu":[]

}

problem_size = [
    "3x3",
    "3x4",
    "4x4",
    "5x4",
    # "5x5"
    # "10x10",
    # "15x15"
    
]

for repeat in range(1,25):
    for method_name in statistics:
        for problem in problem_size:
    
                output_name = "output.txt"
                cmndName = "nonogram --output " + str(output_name) + " --input " + str(problem) +  ".txt --method " + method_name + " --iterations 10000 --tabusize 2048"
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
                microseconds = 0
                nanoseconds = 0
                
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
                        microseconds = int(unit_value)
                    elif "us" == unit_name:
                        nanoseconds = int(unit_value)
    
    
                    seconds_total = (hours*3600)+(minutes*60)+seconds
                    if microseconds > 0 or nanoseconds > 0:
                        seconds_total += 1

    #            calcTime = re.findall("dt.*", output)
    #            if (len(calcTime) > 0):
    #                calcTime = re.findall("[0-9.]+",calcTime[0])
    #                result_val = re.findall("[0-9.]+",re.findall("result.*", output)[0])
    #
                    statistics[method_name].append([problem, float(cost[0]), float(seconds_total)])


print(statistics)
# with open("result.plt", "a") as gnuplotfile:
#     gnuplotfile.write("set term png\n")
#     gnuplotfile.write("set output \"result.png\"\n") 
#     gnuplotfile.write("plot ")
#     for method_name in statistics:
#         print(method_name)
#         summary = statistics[method_name]
#         # print(summary)
#         per_size = {}
#         for values in summary:
#             if (per_size.get(values[0]) is None):
#                 per_size[values[0]] = [[values[1], values[2]]] 
#             else:
#                 per_size[values[0]].append([values[1], values[2]])
#         #print(per_size)
#         for s in per_size:
#             combined = np.mean(per_size[s], axis=0)
#             with open("result_" + method_name + ".txt", "a") as myfile:
#                 myfile.write(str(s) + " "  + str(combined[0]) + " "+ str(combined[1]) + "\n")
#         gnuplotfile.write("'result_" + method_name + ".txt' u 1:2 w lines, ") 

#     gnuplotfile.write("\n") 


# result = os.popen("gnuplot result.plt")
# output = result.read()