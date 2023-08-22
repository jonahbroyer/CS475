#!/bin/bash
#SBATCH -J AutoCorr
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 8   # number of nodes
#SBATCH -n 8   # number of tasks
#SBATCH --constraint=ib
#SBATCH -o autocorr.out
#SBATCH -e autocorr.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=broyerj@oregonstate.edu

for t in 1 2 4 8
do
    module load openmpi
    mpic++ autocorr.cpp -o autocorr -lm
    mpiexec -mca btl self,tcp -np $t ./autocorr
done
