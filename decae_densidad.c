/*Xibelly Eliseth Mosquera Escobar
 * 
 * programa: decae_densidad.c
 * 
 * Este programa lee los datos obtenidos al hacer la interpolacion y busca para que valor     
 *  del radio donde la densidad a 
 * empezado a decaer en un fator de e.
 */


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
  double r_e;	

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
	  
	  r_e = ri[i];
	  printf("RADIUS WHERE DECREASE THE DENSITY IS: %lf\n",r_e);
	  break;
	}
    }
  
  
  




  return 0;
}
