printinfo: printinfo.cpp
	g++ -o printinfo printinfo.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp

first: first.cpp
	g++ -o first first.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp

second: second.cpp
	g++ -o second second.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp

clean:
	rm -f *~ *#
