#!/bin/csh
echo "NUM_ELEMENTS, LOCAL_SIZE, GigaMultsReducePerSecond"

foreach l (32 64 128 256)
    foreach n (1 2 4 8)
        g++ -o second second.cpp -DLOCAL_SIZE=$l -DNMB=$n /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
        ./second
    end
end
