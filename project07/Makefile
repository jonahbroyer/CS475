autocorr:	autocorr.cpp
			/usr/local/apps/openmpi/3.1_gcc-8.3/bin/mpic++ autocorr.cpp -o autocorr -lm

run:		autocorr
			/usr/local/apps/openmpi/3.1_gcc-8.3/bin/mpiexec -np 8 ./autocorr

clean:
	rm -f *~ *#
