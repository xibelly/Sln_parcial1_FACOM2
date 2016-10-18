set terminal png
set output "grafica_distribucion.png" 
set pointsize 1.5
set xlabe 'X'
set ylabe 'Y'
set key top right
plot "coordenadas.dat"  u ($1):($2) title 'Distribución' 
