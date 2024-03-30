import os
import shutil

def analizeFile(sourceFile,resultFile):#analize the source file and save the information in the result file
    f=open(sourceFile,"r")#open file and read all its lines
    lines=f.readlines()
    data={}
    num={}
    for line in lines:#for each line get the informations
        type,omp,cuda,creationTime,executionTime,totalTime=line.strip()[:-1].split(";")
        if type not in data:
            data[type]={}
            num[type]={}
        if omp not in data[type]:
            data[type][omp]={}
            num[type][omp]={}
        if cuda not in data[type][omp]:
            data[type][omp][cuda]=[float(creationTime),float(executionTime),float(totalTime)]
            num[type][omp][cuda]=1
        else:
            data[type][omp][cuda][0]+=float(creationTime)
            data[type][omp][cuda][1]+=float(executionTime)
            data[type][omp][cuda][2]+=float(totalTime)
            num[type][omp][cuda]+=1
    f.close()#close the source file and open the result file
    f=open(resultFile,"w")

    f.write("Modality;OMP;BlockSize;String_generation_time;Kernel_edit_distance_time;Program_execution_time;speedup;efficiency\n")
    for typeKey in data.keys():#for each element in the dictionary calculate the mean and save it in the result file
        ompList=list(data[typeKey].keys())
        ompList.sort(key=int)
        for ompKey in ompList:
            cudaList=list(data[typeKey][ompKey].keys())
            cudaList.sort()
            for cudaKey in cudaList:
                it = num[typeKey][ompKey][cudaKey]
                data[typeKey][ompKey][cudaKey][0] /= it
                data[typeKey][ompKey][cudaKey][1] /= it
                data[typeKey][ompKey][cudaKey][2] /= it

                speedup=1
                efficiency=100
                if typeKey != "Approximate":
                    speedup = data["Approximate"][ompKey][cudaKey][2] / data[typeKey][ompKey][cudaKey][2]
                    efficiency = speedup / int(ompKey)*100
                
                formatted_line = (
                    f"{typeKey};{ompKey};{cudaKey};"
                    f"{data[typeKey][ompKey][cudaKey][0]:.8f};"
                    f"{data[typeKey][ompKey][cudaKey][1]:.8f};"
                    f"{data[typeKey][ompKey][cudaKey][2]:.8f};"
                    f"{speedup:.8f};{efficiency:.4f}\n"
                )
                f.write(formatted_line)

    f.close()#close the result file

for path, currentDirectory, files in os.walk("Informations/OMP_CUDA"):#for each file in the directory Informations, analize them and save the results in the relative path in Results directory
    files = sorted(files)
    for file in files:
        resultPath=path.replace("Informations","Results")
        if not(os.path.exists(resultPath)):
             os.makedirs(resultPath)
        analizeFile(os.path.join(path, file),os.path.join(resultPath,file))           