#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_sort_int.h>
#include <gsl/gsl_sort_double.h>

#define NOM_ARCHIVO "inter_densidad.dat"

int decae_densidad()
{
  int i, nread;
  int ch, num_lineas;
  double e, a;
  double *radio, *densidad;
  double *ri, *iter;

  FILE *pf1 = NULL;
  FILE *entrada =NULL;

  e = exp(1);
  
 
  //# de lineas del archivo inter_densidad.dat
  if ((entrada = fopen("inter_densidad.dat", "r")) == NULL){
    perror(NOM_ARCHIVO);
    return EXIT_FAILURE;
   }
  
  num_lineas = 0;
  while ((ch = fgetc(entrada)) != EOF)
    if (ch == '\n')
      num_lineas++;
  
 
  printf("# LINES OF INTERPOLATION FILE: %d\n", num_lineas);
  
  size_t p[num_lineas];

  int k = num_lineas;

  ri  = (double *) malloc(num_lineas *sizeof(double));
   
  iter  = (double *) malloc(num_lineas *sizeof(double));

  densidad = (double *) malloc(num_lineas *sizeof(double));

  radio = (double *) malloc(num_lineas *sizeof(double));
  
  
 
  pf1 = fopen("inter_densidad.dat", "r");

  for(i=0; i<num_lineas; i++)
    {
      nread = fscanf(pf1,"%lf %lf",&ri[i], &iter[i]);

    }

  for(i=0; i<num_lineas; i++)
    {  
      
      a = iter[i];
      
      if(a < e)
	{
	  densidad[i] = a;
	  
	}
    }
  //gsl_sort_smallest_index (p, k, densidad, 1, num_lineas);//ordenamos dichos valores en forma creciente
  
  printf("RADIUS WHERE DECREASE THE DENSITY IS: %lf\n",ri[0]);




  return 0;
}
