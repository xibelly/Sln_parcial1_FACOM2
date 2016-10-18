set terminal png
set output "grafica_triangulos.png" 
set pointsize 1.5
set xlabe 'Distancia centro sistema'
set ylabe '#triangulos'
set key top right
plot "triangulos-centro.dat"  u ($2):($1) title '#Triangulos' 
