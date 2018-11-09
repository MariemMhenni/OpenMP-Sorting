#!/bin/bash

# Optimal number of software threads for the current program = number of hardware threads.
omp_num_threads=`cat /proc/cpuinfo | grep processor | wc -l`
# Optimal number of blocks determined by an empiric formula, depend on data size (N*K).
n_exp='echo "scale=0 ; e(l(${data_size[$i]})/l(10))" | bc -l'
# The total data size (N*K) cases to test.
data_size=(10 30 100 300 1000 3000 10000 30000 100000 300000 1000000 3000000 10000000)
# Path to the program.
prg=./ompsort

# Execution time for an optimal N, optimal number of threads, and a dynamic
# K.
# ==============================================================================

# Data specific to this run.
# Output file.
output=./report/charts/chart1.csv

# Header.
echo 'data size,exec time' > $output

# Data.
for ((i = 0; i < ${#data_size[@]}; i++)); do
    echo -n "${data_size[$i]}," >> $output
    n=`eval $n_exp`
    OMP_NUM_THREADS=$omp_num_threads $prg $n $((${data_size[$i]} / $n)) | sed '2!d' >> $output
done

# Execution time for an optimal N, a determined varying number of threads
# and a dynamic K.
# ==============================================================================

# Data specific to this run.
# Output file.
output=./report/charts/chart3.csv
# Set of N parameter to test for each data size.
omp_num_threads_tab=(1 4 16 64)

# Header.
echo -n 'data size' > $output
for ((i = 0; i < ${#omp_num_threads_tab[@]}; i++)); do
    echo -n ",exec time omp_num_threads eq ${omp_num_threads_tab[$i]}" >> $output
done
echo "" >> $output

# Data.
for ((i = 0; i < ${#data_size[@]}; i++)); do
    echo -n "${data_size[$i]}" >> $output
    for ((j = 0; j < ${#omp_num_threads_tab[@]}; j++)); do
        echo -n "," >> $output
        n=`eval $n_exp`
        OMP_NUM_THREADS=${omp_num_threads_tab[$j]} $prg $n $((${data_size[$i]} / $n)) | sed '2!d' | tr \\n \000 >> $output
    done
    echo "" >> $output
done

# Execution time for a determined varying N, an optimal number of threads
# and a dynamic K.
# ==============================================================================

# Data specific to this run.
# Output file.
output=./report/charts/chart2.csv
# Set of N parameter to test for each data size.
n_tab=(2 16 64 256 1024)
# The total data size (N*K) cases to test.
data_size=(10 30 100 300 1000 3000 10000 30000 100000 300000 1000000)

# Header.
echo -n 'data size' > $output
for ((i = 0; i < ${#n_tab[@]}; i++)); do
    echo -n ",exec time n eq ${n_tab[$i]}" >> $output
done
echo "" >> $output

# Data.
for ((i = 0; i < ${#data_size[@]}; i++)); do
    echo -n "${data_size[$i]}" >> $output
    for ((j = 0; j < ${#n_tab[@]}; j++)); do
        echo -n "," >> $output
        n=${n_tab[$j]}
        OMP_NUM_THREADS=$omp_num_threads $prg $n $((${data_size[$i]} / $n)) 2>/dev/null > /tmp/ompsort
        # Print a 0 if there is an error, in this case if the N is superior to N*K.
        if [[ $? -eq 0 ]]; then
            sed '2!d' /tmp/ompsort | tr \\n \000 >> $output
        else
            echo -n "0" >> $output
        fi
    done
    echo "" >> $output
done
