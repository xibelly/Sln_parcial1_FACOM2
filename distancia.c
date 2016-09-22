/*Xibelly Eliseth Mosquera Escobar

Este programa se usa para calcular la distancia entre particulas

 */
#include<math.h>

double distancia(double xi, double yi, double xj, double yj)
{

  double dist, dx, dy;
  
  dx = (xi-xj)*(xi-xj);
  dy = (yi-yj)*(yi-yj);
  
  
  dist = sqrt(dx + dy); 
  
  return dist;

}