#!/bin/csh
echo "NUM_ELEMENTS, LOCAL_SIZE, GigaMultsPerSecond"

foreach l (8 16 32 64 128 256 512)
    foreach n (1 2 4 8)
        g++ -o first first.cpp -DLOCAL_SIZE=$l -DNMB=$n /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
        ./first
    end
end
