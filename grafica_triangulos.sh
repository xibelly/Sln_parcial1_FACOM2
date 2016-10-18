#!/bin/bash 

echo "Introduce the data1:"
read data1

echo "Introduce the output file:"
read archivo_salida

echo "Introduce signo-pesos1"
read col

echo "Introduce signo-pesos2"
read col2


echo "set terminal png" > script_grafica_triangulos.gp
echo "set output "$archivo_salida" " >> script_grafica_triangulos.gp
echo "set pointsize 1.5" >> script_grafica_triangulos.gp
echo "set xlabe 'Distancia centro sistema'"  >> script_grafica_triangulos.gp
echo "set ylabe '#triangulos'"  >> script_grafica_triangulos.gp
echo "set key top right" >> script_grafica_triangulos.gp

echo "plot "$data1"  u ("$col2"):("$col") title '#Triangulos' "  >> script_grafica_triangulos.gp

gnuplot script_grafica_triangulos.gp

