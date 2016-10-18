plot "densidad_distancia.dat"  u ($1):($2) w lp  lc rgb 'red' title 'Densiadad-distancia' 
set terminal png
set output "grafica_densidad.png" 
set pointsize 1.5
set xlabe 'Rbin'
set ylabe 'ρ'
set key top right
replot "inter_densidad.dat"  u ($1):($2) w lp  lc rgb 'blue' title 'Interpolación' 
