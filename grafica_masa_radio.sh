#!/bin/bash 

echo "Introduce the data1:"
read data1

echo "Introduce the output file:"
read archivo_salida

echo "Introduce signo-pesos1"
read col

echo "Introduce signo-pesos2"
read col2


echo "set terminal png" > script_grafica_masa_radio.gp
echo "set output "$archivo_salida" " >> script_grafica_masa_radio.gp
echo "set pointsize 1.5" >> script_grafica_masa_radio.gp
echo "set xlabe 'Rbines'"  >> script_grafica_masa_radio.gp
echo "set ylabe 'Masa'"  >> script_grafica_masa_radio.gp
echo "set key top right" >> script_grafica_masa_radio.gp

echo "plot "$data1"  u ("$col"):("$col2") title 'Masa-Radio' "  >> script_grafica_masa_radio.gp

gnuplot script_grafica_masa_radio.gp

