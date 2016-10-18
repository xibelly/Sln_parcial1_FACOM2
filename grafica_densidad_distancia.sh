#!/bin/bash 

echo "Introduce the data1:"
read data1

echo "Introduce the data2:"
read data2

echo "Introduce the output file:"
read archivo_salida

echo "Introduce signo-pesos1"
read col

echo "Introduce signo-pesos2"
read col2


echo "plot "$data1"  u ("$col"):("$col2") w lp  lc rgb 'red' title 'Densiadad-distancia' "  > script_grafica_densidad.gp


echo "set terminal png" >> script_grafica_densidad.gp
echo "set output "$archivo_salida" " >> script_grafica_densidad.gp
echo "set pointsize 1.5" >> script_grafica_densidad.gp
echo "set xlabe 'Rbin'"  >> script_grafica_densidad.gp
echo "set ylabe 'ρ'"  >> script_grafica_densidad.gp
echo "set key top right" >> script_grafica_densidad.gp

echo "replot "$data2"  u ("$col"):("$col2") w lp  lc rgb 'blue' title 'Interpolación' "  >> script_grafica_densidad.gp

gnuplot script_grafica_densidad.gp

