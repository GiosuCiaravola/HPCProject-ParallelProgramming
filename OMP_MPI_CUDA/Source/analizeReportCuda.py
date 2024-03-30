import os
import shutil

def analizeFile(sourceFile,resultFile):#analize the source file and save the information in the result file
    unique_lines_dict = {}
    f=open(sourceFile,"r")#open file and read all its lines
    for line in f:
        unique_lines_dict[line] = True

    f.close()#close the source file and open the result file

    f=open(resultFile,"w")

    for unique_line in unique_lines_dict.keys():
        f.write(unique_line)

    f.close()#close the result file

for path, currentDirectory, files in os.walk("EditDistanceReport/OMP_CUDA"):#for each file in the directory Informations, analize them and save the results in the relative path in Results directory
    files = sorted(files)
    for file in files:
        resultPath=path.replace("EditDistanceReport","DistinctEditDistanceReport")
        if not(os.path.exists(resultPath)):
            os.makedirs(resultPath)
        analizeFile(os.path.join(path, file),os.path.join(resultPath,file))            