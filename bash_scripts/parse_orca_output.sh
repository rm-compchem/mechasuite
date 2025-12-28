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


for fpath in $*; do
    # check whether it is a dir and try to find input_trj.xyz
    if [ -d $fpath ];then
        if [ -s ${fpath}/input_trj.xyz ];then
		fpath=${fpath}/input_trj.xyz
	elif [ -s ${fpath}/react.xyz ]; then
	        fpath=${fpath}/react.xyz
	else
		continue
	fi
    fi

    echo $fpath
    n=$(head -1 $fpath)
    let "n=n+2"
    dname=$(dirname $fpath)
    #echo $dname/opt.xyz $n 
    tail -n $n $fpath > $dname/opt.xyz

    # override if it is a neb
    if [ -s $dname/input_NEB-CI_converged.xyz ];then
        cp $dname/input_NEB-CI_converged.xyz $dname/opt.xyz

    elif [ -s $dname/input_MEP.allxyz ];then
        imgname=$(get_ts $dname/input_MEP.allxyz)
        echo $imgname '| for |' $dname
        if [ -s $dname/$imgname ]; then
            cp $dname/$imgname $dname/opt.xyz
        fi
    fi

    echo "program orca " > $dname/.data
    energy=$(grep E $dname/opt.xyz | awk '{print $NF}')
    echo "energy $energy" >> $dname/.data
    echo "struct opt.xyz" >> $dname/.data
    echo "unit Ha" >> $dname/.data
    echo "tp min" >> $dname/.data


done
