set terminal png
set output "grafica_masa.png" 
set pointsize 1.5
set xlabe 'Rbines'
set ylabe 'Masa'
set key top right
plot "Masa_Acumulada.dat"  u ($1):($2) title 'Masa-Radio' 
