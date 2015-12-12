#!/bin/bash

# Corre compare.py en baby.avi con el metodo de splines y diferentes valores
# de reset.

out='measureTimes/baby_reset_ecm.txt'
echo 'Reset,Salto,PromECM' > $out

for reset in 4 8 12 16 20; do
	for jump in 2 4 6 8 10 11 12; do
		echo -n "$reset,$((jump - 1))," >> $out
		python compare.py data/baby.avi "output/baby_splines_${jump}_${reset}.avi" --method interpolacionPorSplines --jump $jump --reset $reset |
		grep 'ECM' | grep -o '[0-9.]*' >> $out
	done;
done;
