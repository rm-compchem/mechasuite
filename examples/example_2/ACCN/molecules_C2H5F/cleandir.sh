#!/bin/bash
shopt -s extglob

rm !(cleandir.sh|params.yaml|job.sh|*.xyz|plumed.dat|job.inp|) 2>/dev/null

