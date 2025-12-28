#!/bin/bash
#

function get_ts {
  n=$(head -1 $1)
  #maxline=$(grep E $1 | awk '{  if ($3 > max_val) {    max_val = $3;    max_line = NR;  }}END {  print max_line;}' )

maxline=$(grep E $1 | awk '
BEGIN { max_val=-1e100; max_line=0 }
{
    if ($NF > max_val) {
        max_val = $NF
        max_line = NR   # store the line number
    }
}
END { print max_line }
')


let "maxline=maxline-1" # orca writes images from zero
echo input_im${maxline}.xyz

}


for dname in $*; do
    # check whether it is a dir and try to find input_trj.xyz
    if [ -d $dname ];then
        if [ -s ${dname}/CONTCAR ];then
		fpath=CONTCAR
	else
	        continue
	fi
    fi

    if [ -s ${dname}/OSZICAR ];then
	    energyfile=OSZICAR
    else
	    continue
    fi
    # check TS
    #
    if [ -s ${dname}/DIMER ];then
	   tp=ts 
    else
	   tp=min
    fi

    echo $dname
    echo "program vasp " > $dname/.data
    echo "energy $energyfile" >> $dname/.data
    echo "struct $fpath" >> $dname/.data
    echo "unit eV" >> $dname/.data
    echo "tp $tp" >> $dname/.data
    echo "pg solid" >> $dname/.data

    # manage freqs
    if [ -s $dname/freq/OUTCAR ];then
        echo "freq freq/OUTCAR" >> $dname/.data        
    else
        grep -q VIBRATIONAL $dname/OUTCAR && echo "freq OUTCAR " >> $dname/.data
   fi 

done
