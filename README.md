# GPU-DFC

This repository contains Fast-GPU-PCC algorithm

# Compilaton
nvcc -rdc=true -lcusparse -lcusolver -lcublas -O2 -arch=compute_35 -code=sm_35 -std=c++11 -Xptxas="-dlcm=ca" CPU_side.cpp GPU_side_region.cu GPU_side_decomposition.cu GPU_side_sparse.cu -o out

# Running

./out N L W S 1

N: number of regions/voxel

L: length of time series

W: window size

S: step size

Method: Memory reduction strategy (1: no memory reduction, 2: Matrix decomposition, 3: sparsification)
    
    
The data should be stored in row major format (first N elements corresponds to time series of first element, second N elements corresponds to time series of first element and …)

