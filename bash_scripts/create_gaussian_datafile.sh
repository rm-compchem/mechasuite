#!/bin/bash
#

for dname in $*; do
    # check whether it is a dir and try to find input_trj.xyz
    if [ ! -d $dname ];then
	    continue
    fi

    energyfile=input.log
    fpath=input.log

    echo $dname
    echo "program gaussian " > $dname/.data
    echo "energy $energyfile" >> $dname/.data
    echo "struct $fpath" >> $dname/.data
    echo "unit Ha" >> $dname/.data
    echo "tp $tp" >> $dname/.data

done
