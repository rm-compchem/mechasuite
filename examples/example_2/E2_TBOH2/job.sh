#!/bin/bash
# The name to show in queue lists for this job:
#SBATCH --error=serial.%J.err
#SBATCH --output=serial.%J.out

# Number of desired cpus:
#SBATCH --ntasks=16
# Per seleccionar nodos
#SBATCH --constraint=cal
# Amount of RAM needed for this job:
#SBATCH --mem=128GB
#SBATCH -N 1

# The time the job will be running:
#SBATCH --time=24:00:00

# To load some software (you can show the list with 'module avail'):
module load orca/6.0.0
# the program to execute with its parameters:
hostname

DIR=${SLURM_SUBMIT_DIR}
echo ${SLURM_JOB_ID} > ${DIR}/_slurmjobid
#WORKDIR=${TMPDIR}/${SLURM_JOB_ID}__${USER}
WORKDIR=/mnt/home/users/itq38_res/resh000475/fscratch/miscr/${SLURM_JOB_ID}__${USER}
mkdir -p ${WORKDIR}

cd ${WORKDIR}

cp ${DIR}/job.inp ./job.inp

sleep 2

srun orca job.inp >> job.out

sleep 2

    
cp job.out ${DIR}/job.out
cp job.xyz ${DIR}/job.xyz
cp job.gbw ${DIR}/job.gbw




