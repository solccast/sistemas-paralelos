#!/bin/bash 

#SBATCH --exclusive

# Tamaños de matriz validos 
nxn=(512 1024 2048 4096)

# Cantidad de hilos 
hilos=(2 4 8)

for n in "${nxn[@]}"; do
    for h in "${hilos[@]}"; do
        output_file="./output_openmp/output_${n}_${h}_o3_fix_barrier_1.txt"
        error_file="./output_openmp/errors_${n}_${h}_o3_fix_barrier_1.txt"
        ./ejercicio_openmp_sin_collapse "$n" "$h" > "$output_file" 2> "$error_file"
    done
done