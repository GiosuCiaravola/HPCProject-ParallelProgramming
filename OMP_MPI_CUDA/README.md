## Dependencies

* Python3
* OMP
* R

## How to run
Premise: The code for generating plots requires the python interpreter and two libraries to be installed, matplotlib, and pandas with the following commands:
pip3 install matplotlib
pip install pandas

When generating tables with R a package will be installed, known as kableExtra and webshot.

1.	Navigate to the project folder containing the makefile

2.	To clear previously obtained achievements and previous builds, enter the command
make clean

3.	To generate the necessary directories and compile and linking the various source codes, both for mpi and cuda enter the command
make all

4. To separate the compilation onlu for mpi enter the commands
make compile_mpi
make compile_cuda

5. To make the tests use the commands
make test

6. To test separately mpi and cuda enter the commands
make mpi_test
make cuda_test

7. To produce plots and tables for mpi enter the command
make mpi_plots

9.	(OMP+CUDA) To produce plots and tables enter the command
make cuda_plots

The folder contains all the results used for the report. To clean view point 1.
The results of the algorithms can be viewed in the "EditDistance_Report" folder, which has three subfolders depending on the size of the problem (the characters of the strings).
The execution times of the algorithms and their average values can be viewed respectively in the "Informations" and "Results" folders, divided by optimization.
The results in graphical and tabular format can be viewed respectively in the "Plots" and "Tables" folders, divided by optimization.
To reduce the completion time, change the value of "iterations" in the makefile to the desired number (each optimization iterations lasts about 9 minutes).