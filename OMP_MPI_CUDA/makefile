.PHONY: compile_mpi compile_cuda all clean mpi_test cuda_test test mpi_compile0 mpi_compile1 mpi_compile2 mpi_compile3 cuda_compile0 cuda_compile1 cuda_compile2 cuda_compile3 mpi_test0 mpi_test1 mpi_test2 mpi_test3 cuda_test0 cuda_test1 cuda_test2 cuda_test3

all: create_directories compile_mpi compile_cuda

compile_mpi: mpi_compile0 mpi_compile1 mpi_compile2 mpi_compile3

compile_cuda: cuda_compile0 cuda_compile1 cuda_compile2 cuda_compile3

clean:
	rm -rf ./Build/*
	rm -rf ./Informations
	rm -rf ./EditDistanceReport
	rm -rf ./DistinctEditDistanceReport
	rm -rf ./Results
	rm -rf ./Tables
	rm -rf ./Plots

mpi_test: mpi_test0 mpi_test1 mpi_test2 mpi_test3

cuda_test: cuda_test0 cuda_test1 cuda_test2 cuda_test3

test: mpi_test cuda_test

n_characters := 10000 20000 40000
omp_threads := 1 2 4 8
mpi_process := 1 2 4
seed1 = 18723
seed2 = 1
iterations := 1 2 3 4 5 6 7

create_directories:
	@echo "Creating directories..."
	@mkdir -p Informations
	@cd Informations && mkdir -p OMP_MPI && cd OMP_MPI && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@cd Informations && mkdir -p OMP_CUDA && cd OMP_CUDA && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@mkdir -p EditDistanceReport
	@cd EditDistanceReport && mkdir -p OMP_MPI && cd OMP_MPI && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@mkdir -p DistinctEditDistanceReport
	@cd DistinctEditDistanceReport && mkdir -p OMP_MPI && cd OMP_MPI && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@cd EditDistanceReport && mkdir -p OMP_CUDA && cd OMP_CUDA && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@mkdir -p Results
	@cd Results && mkdir -p OMP_MPI && cd OMP_MPI && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@cd Results && mkdir -p OMP_CUDA && cd OMP_CUDA && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@mkdir -p Tables
	@cd Tables && mkdir -p OMP_MPI && cd OMP_MPI && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@cd Tables && mkdir -p OMP_CUDA && cd OMP_CUDA && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@mkdir -p Plots
	@cd Plots && mkdir -p OMP_MPI && cd OMP_MPI && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done
	@cd Plots && mkdir -p OMP_CUDA && cd OMP_CUDA && \
		for i in {0..3}; do \
			mkdir -p opt$$i; \
		done

	@echo "Directories created!"

mpi_compile0:
	gcc -c -o ./Build/Random_String_Generator.o ./Source/Random_String_Generator.c -O0
	gcc -c -o ./Build/Edit_Distance_Sequential.o ./Source/Edit_Distance_Sequential.c -O0
	gcc -c -o ./Build/Edit_Distance_Sequential_Approximate.o ./Source/Edit_Distance_Sequential_Approximate.c -O0
	mpicc -c -o ./Build/Edit_Distance_OMP_MPI.o ./Source/Edit_Distance_OMP_MPI.c -fopenmp -O0

	gcc -o ./Build/Edit_Distance_Sequential0.exe ./Build/Edit_Distance_Sequential.o ./Build/Random_String_Generator.o -O0
	gcc -o ./Build/Edit_Distance_Sequential_Approximate0.exe ./Build/Edit_Distance_Sequential_Approximate.o ./Build/Random_String_Generator.o -O0
	mpicc -o ./Build/Edit_Distance_OMP_MPI0.exe ./Build/Edit_Distance_OMP_MPI.o ./Build/Random_String_Generator.o -fopenmp -O0

mpi_compile1:
	gcc -c -o ./Build/Random_String_Generator.o ./Source/Random_String_Generator.c -O1
	gcc -c -o ./Build/Edit_Distance_Sequential.o ./Source/Edit_Distance_Sequential.c -O1
	gcc -c -o ./Build/Edit_Distance_Sequential_Approximate.o ./Source/Edit_Distance_Sequential_Approximate.c -O1
	mpicc -c -o ./Build/Edit_Distance_OMP_MPI.o ./Source/Edit_Distance_OMP_MPI.c -fopenmp -O1

	gcc -o ./Build/Edit_Distance_Sequential1.exe ./Build/Edit_Distance_Sequential.o ./Build/Random_String_Generator.o -O1
	gcc -o ./Build/Edit_Distance_Sequential_Approximate1.exe ./Build/Edit_Distance_Sequential_Approximate.o ./Build/Random_String_Generator.o -O1
	mpicc -o ./Build/Edit_Distance_OMP_MPI1.exe ./Build/Edit_Distance_OMP_MPI.o ./Build/Random_String_Generator.o -fopenmp -O1

mpi_compile2:
	gcc -c -o ./Build/Random_String_Generator.o ./Source/Random_String_Generator.c -O2
	gcc -c -o ./Build/Edit_Distance_Sequential.o ./Source/Edit_Distance_Sequential.c -O2
	gcc -c -o ./Build/Edit_Distance_Sequential_Approximate.o ./Source/Edit_Distance_Sequential_Approximate.c -O2
	mpicc -c -o ./Build/Edit_Distance_OMP_MPI.o ./Source/Edit_Distance_OMP_MPI.c -fopenmp -O2

	gcc -o ./Build/Edit_Distance_Sequential2.exe ./Build/Edit_Distance_Sequential.o ./Build/Random_String_Generator.o -O2
	gcc -o ./Build/Edit_Distance_Sequential_Approximate2.exe ./Build/Edit_Distance_Sequential_Approximate.o ./Build/Random_String_Generator.o -O2
	mpicc -o ./Build/Edit_Distance_OMP_MPI2.exe ./Build/Edit_Distance_OMP_MPI.o ./Build/Random_String_Generator.o -fopenmp -O2

mpi_compile3:
	gcc -c -o ./Build/Random_String_Generator.o ./Source/Random_String_Generator.c -O3
	gcc -c -o ./Build/Edit_Distance_Sequential.o ./Source/Edit_Distance_Sequential.c -O3
	gcc -c -o ./Build/Edit_Distance_Sequential_Approximate.o ./Source/Edit_Distance_Sequential_Approximate.c -O3
	mpicc -c -o ./Build/Edit_Distance_OMP_MPI.o ./Source/Edit_Distance_OMP_MPI.c -fopenmp -O3

	gcc -o ./Build/Edit_Distance_Sequential3.exe ./Build/Edit_Distance_Sequential.o ./Build/Random_String_Generator.o -O3
	gcc -o ./Build/Edit_Distance_Sequential_Approximate3.exe ./Build/Edit_Distance_Sequential_Approximate.o ./Build/Random_String_Generator.o -O3
	mpicc -o ./Build/Edit_Distance_OMP_MPI3.exe ./Build/Edit_Distance_OMP_MPI.o ./Build/Random_String_Generator.o -fopenmp -O3

cuda_compile0:
	gcc -c -o ./Build/Random_String_Generator.o ./Source/Random_String_Generator.c -O0
	gcc -c -o ./Build/Edit_Distance_Sequential.o ./Source/Edit_Distance_Sequential.c -O0
	gcc -c -o ./Build/Edit_Distance_Sequential_Approximate_Cuda.o ./Source/Edit_Distance_Sequential_Approximate_Cuda.c -O0

	gcc -o ./Build/Edit_Distance_Sequential0.exe ./Build/Edit_Distance_Sequential.o ./Build/Random_String_Generator.o -O0
	gcc -o ./Build/Edit_Distance_Sequential_Approximate_Cuda0.exe ./Build/Edit_Distance_Sequential_Approximate_Cuda.o ./Build/Random_String_Generator.o -O0

	nvcc -Xcompiler -openmp -o ./Build/Edit_Distance_OMP_CUDA0.exe ./Source/Edit_Distance_OMP_CUDA.cu -O0
	nvcc -Xcompiler -openmp -o ./Build/Edit_Distance_OMP_CUDA0L1.exe ./Source/Edit_Distance_OMP_CUDA.cu -D L1_CACHE -O0

cuda_compile1:
	gcc -c -o ./Build/Random_String_Generator.o ./Source/Random_String_Generator.c -O1
	gcc -c -o ./Build/Edit_Distance_Sequential.o ./Source/Edit_Distance_Sequential.c -O1
	gcc -c -o ./Build/Edit_Distance_Sequential_Approximate_Cuda.o ./Source/Edit_Distance_Sequential_Approximate_Cuda.c -O1

	gcc -o ./Build/Edit_Distance_Sequential1.exe ./Build/Edit_Distance_Sequential.o ./Build/Random_String_Generator.o -O1
	gcc -o ./Build/Edit_Distance_Sequential_Approximate_Cuda1.exe ./Build/Edit_Distance_Sequential_Approximate_Cuda.o ./Build/Random_String_Generator.o -O1

	nvcc -Xcompiler -openmp -o ./Build/Edit_Distance_OMP_CUDA1.exe ./Source/Edit_Distance_OMP_CUDA.cu -O1
	nvcc -Xcompiler -openmp -o ./Build/Edit_Distance_OMP_CUDA1L1.exe ./Source/Edit_Distance_OMP_CUDA.cu -D L1_CACHE -O1

cuda_compile2:
	gcc -c -o ./Build/Random_String_Generator.o ./Source/Random_String_Generator.c -O2
	gcc -c -o ./Build/Edit_Distance_Sequential.o ./Source/Edit_Distance_Sequential.c -O2
	gcc -c -o ./Build/Edit_Distance_Sequential_Approximate_Cuda.o ./Source/Edit_Distance_Sequential_Approximate_Cuda.c -O2

	gcc -o ./Build/Edit_Distance_Sequential2.exe ./Build/Edit_Distance_Sequential.o ./Build/Random_String_Generator.o -O2
	gcc -o ./Build/Edit_Distance_Sequential_Approximate_Cuda2.exe ./Build/Edit_Distance_Sequential_Approximate_Cuda.o ./Build/Random_String_Generator.o -O2

	nvcc -Xcompiler -openmp -o ./Build/Edit_Distance_OMP_CUDA2.exe ./Source/Edit_Distance_OMP_CUDA.cu -O2
	nvcc -Xcompiler -openmp -o ./Build/Edit_Distance_OMP_CUDA2L1.exe ./Source/Edit_Distance_OMP_CUDA.cu -D L1_CACHE -O2

cuda_compile3:
	gcc -c -o ./Build/Random_String_Generator.o ./Source/Random_String_Generator.c -O3
	gcc -c -o ./Build/Edit_Distance_Sequential.o ./Source/Edit_Distance_Sequential.c -O3
	gcc -c -o ./Build/Edit_Distance_Sequential_Approximate_Cuda.o ./Source/Edit_Distance_Sequential_Approximate_Cuda.c -O3

	gcc -o ./Build/Edit_Distance_Sequential3.exe ./Build/Edit_Distance_Sequential.o ./Build/Random_String_Generator.o -O3
	gcc -o ./Build/Edit_Distance_Sequential_Approximate_Cuda3.exe ./Build/Edit_Distance_Sequential_Approximate_Cuda.o ./Build/Random_String_Generator.o -O3

	nvcc -Xcompiler -openmp -o ./Build/Edit_Distance_OMP_CUDA3.exe ./Source/Edit_Distance_OMP_CUDA.cu -O3
	nvcc -Xcompiler -openmp -o ./Build/Edit_Distance_OMP_CUDA3L1.exe ./Source/Edit_Distance_OMP_CUDA.cu -D L1_CACHE -O3

mpi_test0:
	@opt=0; \
	echo "----------------------------{OMP_MPI}----------------------------"; \
	for characters in $(n_characters); do \
		echo "----------------------------{$$characters,$$opt}----------------------------"; \
		./Build/Edit_Distance_Sequential0.exe $$characters $(seed1) $(seed2) $$opt OMP_MPI; \
		for it in $(iterations); do \
			echo "-------------------------------{$$it}-------------------------------"; \
			for omp in $(omp_threads); do \
				for mpi in $(mpi_process); do \
					./Build/Edit_Distance_Sequential_Approximate0.exe $$characters $(seed1) $(seed2) $$opt $$omp $$mpi; \
					mpiexec -n $$mpi ./Build/Edit_Distance_OMP_MPI0.exe $$characters $(seed1) $(seed2) $$opt $$omp -fopenmp; \
				done; \
			done; \
		done; \
	done;

mpi_test1:
	@opt=1; \
	for characters in $(n_characters); do \
		echo "----------------------------{$$characters,$$opt}----------------------------"; \
		./Build/Edit_Distance_Sequential1.exe $$characters $(seed1) $(seed2) $$opt OMP_MPI; \
		for it in $(iterations); do \
			echo "-------------------------------{$$it}-------------------------------"; \
			for omp in $(omp_threads); do \
				for mpi in $(mpi_process); do \
					./Build/Edit_Distance_Sequential_Approximate1.exe $$characters $(seed1) $(seed2) $$opt $$omp $$mpi; \
					mpiexec -n $$mpi ./Build/Edit_Distance_OMP_MPI1.exe $$characters $(seed1) $(seed2) $$opt $$omp -fopenmp; \
				done; \
			done; \
		done; \
	done;

mpi_test2:
	@opt=2; \
	for characters in $(n_characters); do \
		echo "----------------------------{$$characters,$$opt}----------------------------"; \
		./Build/Edit_Distance_Sequential2.exe $$characters $(seed1) $(seed2) $$opt OMP_MPI; \
		for it in $(iterations); do \
			echo "-------------------------------{$$it}-------------------------------"; \
			for omp in $(omp_threads); do \
				for mpi in $(mpi_process); do \
					./Build/Edit_Distance_Sequential_Approximate2.exe $$characters $(seed1) $(seed2) $$opt $$omp $$mpi; \
					mpiexec -n $$mpi ./Build/Edit_Distance_OMP_MPI2.exe $$characters $(seed1) $(seed2) $$opt $$omp -fopenmp; \
				done; \
			done; \
		done; \
	done;

mpi_test3:
	@opt=3; \
	for characters in $(n_characters); do \
		echo "----------------------------{$$characters,$$opt}----------------------------"; \
		./Build/Edit_Distance_Sequential3.exe $$characters $(seed1) $(seed2) $$opt OMP_MPI; \
		for it in $(iterations); do \
			echo "-------------------------------{$$it}-------------------------------"; \
			for omp in $(omp_threads); do \
				for mpi in $(mpi_process); do \
					./Build/Edit_Distance_Sequential_Approximate3.exe $$characters $(seed1) $(seed2) $$opt $$omp $$mpi; \
					mpiexec -n $$mpi ./Build/Edit_Distance_OMP_MPI3.exe $$characters $(seed1) $(seed2) $$opt $$omp -fopenmp; \
				done; \
			done; \
		done; \
	done;

cuda_test0:
	@opt=0; \
	echo "----------------------------{OMP_CUDA}----------------------------"; \
	for characters in $(n_characters); do \
		echo "----------------------------{$$characters,$$opt}----------------------------"; \
		./Build/Edit_Distance_Sequential0.exe $$characters $(seed1) $(seed2) $$opt OMP_CUDA; \
		for it in $(iterations); do \
			echo "-------------------------------{$$it}-------------------------------"; \
			for omp in $(omp_threads); do \
				./Build/Edit_Distance_Sequential_Approximate_Cuda0.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
				./Build/Edit_Distance_OMP_CUDA0.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
				./Build/Edit_Distance_OMP_CUDA0L1.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
			done; \
		done; \
	done;

cuda_test1:
	@opt=1; \
	for characters in $(n_characters); do \
		echo "----------------------------{$$characters,$$opt}----------------------------"; \
		./Build/Edit_Distance_Sequential1.exe $$characters $(seed1) $(seed2) $$opt OMP_CUDA; \
		for it in $(iterations); do \
			echo "-------------------------------{$$it}-------------------------------"; \
			for omp in $(omp_threads); do \
				./Build/Edit_Distance_Sequential_Approximate_Cuda1.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
				./Build/Edit_Distance_OMP_CUDA1.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
				./Build/Edit_Distance_OMP_CUDA1L1.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
			done; \
		done; \
	done;

cuda_test2:
	@opt=2; \
	for characters in $(n_characters); do \
		echo "----------------------------{$$characters,$$opt}----------------------------"; \
		./Build/Edit_Distance_Sequential2.exe $$characters $(seed1) $(seed2) $$opt OMP_CUDA; \
		for it in $(iterations); do \
			echo "-------------------------------{$$it}-------------------------------"; \
			for omp in $(omp_threads); do \
				./Build/Edit_Distance_Sequential_Approximate_Cuda2.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
				./Build/Edit_Distance_OMP_CUDA2.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
				./Build/Edit_Distance_OMP_CUDA2L1.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
			done; \
		done; \
	done;

cuda_test3:
	@opt=3; \
	for characters in $(n_characters); do \
		echo "----------------------------{$$characters,$$opt}----------------------------"; \
		./Build/Edit_Distance_Sequential3.exe $$characters $(seed1) $(seed2) $$opt OMP_CUDA; \
		for it in $(iterations); do \
			echo "-------------------------------{$$it}-------------------------------"; \
			for omp in $(omp_threads); do \
				./Build/Edit_Distance_Sequential_Approximate_Cuda3.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
				./Build/Edit_Distance_OMP_CUDA3.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
				./Build/Edit_Distance_OMP_CUDA3L1.exe $$characters $(seed1) $(seed2) $$opt $$omp; \
			done; \
		done; \
	done;

mpi_plots:
	python3 ./Source/analize.py
	python3 ./Source/analizeReport.py
	python3 ./Source/makeplots.py

cuda_plots:
	python3 ./Source/analizeCuda.py
	python3 ./Source/makeplotsCuda.py