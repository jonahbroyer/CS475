#!/bin/csh

foreach t ( 1 2 4 8 12 16 20 24 32 )
    foreach n ( 1 2 4 8 16 32 64 128 256 512 1024 2048 4096)
        g++ -O3 integration.cpp -DNUMT=$t -DNUMNODES=$n -o integration -lm -fopenmp
    ./integration
    end
end
