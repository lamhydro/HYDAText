#!/bin/bash 
filein=$1
echo ''
echo "Plotting $filein"
echo ''
filename=$(echo "$filein" | sed "s/.*\///")
filenameNoExt=$(echo "$filein" | sed -r "s/.+\/(.+)\..+/\1/")
dir=${filein%/*}
station=$(echo $filename | cut -d"_" -f 1)
variable=${filenameNoExt#*_} 

if [ "$variable" == "DLY_FLOWS" ]; then
	units="(m^3 /s)"
elif [ "$variable" == "DLY_LEVELS" ]; then
	units="(m)" 
elif [ "$variable" == "SED_DLY_SUSCON" ]; then
	units="(mg/l)"
else
	units=""
fi

# Plotting with gnuplot
gnuplot <<- EOF
	set terminal postscript eps color enhanced
	set output "${dir}/${filenameNoExt}.eps"
	set datafile separator "," 
	unset key
	set autoscale fix
	set timefmt '%Y, %m, %d'
	set xdata time
	set format x '%m-%Y' 
	set ylabel "${variable} ${units}"
	set xlabel 'Day'
	set title "At station ${station}"
	plot "${filein}" u 1:4 w lines
#	pause -1
EOF
