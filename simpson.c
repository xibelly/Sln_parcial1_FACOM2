/*Xibelly Eliseth Mosquera Escobar
 * 
 * programa: simpson.c
 * 
 * 
 * Se usa para integrar la densidad superficial de masa.
 * Dicha integracion se lleva a cabo a traves de la regla de Simpson compuesta
 * con. Dicho programa retorna la masa integrada.
 *
 *  
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NOM_ARCHIVO "inter_densidad.dat"
#define denominador 0.333333333

////////////////////////////FUNCION A INTEGRAR//////////////////////////////////
double simpson(double a, double b)
{
  FILE *pf1 = NULL;
  FILE *entrada =NULL;
  int i,  nread;
  int ch, n, num_lineas;
  double h, par, imp, integ, *r_medio;
  double *fx, *fxx;
  
  
  //********** Algoritmo de integracion Simpson compuesto **********//

  
  pf1 = fopen("inter_densidad.dat","r");


  if ((entrada = fopen("inter_densidad.dat", "r")) == NULL){
    perror(NOM_ARCHIVO);
    return EXIT_FAILURE;
   }
  
  num_lineas = 0;
  while ((ch = fgetc(entrada)) != EOF)
    if (ch == '\n')
      num_lineas++;

  
  r_medio  = (double *) malloc(num_lineas *sizeof(double));
  fx  = (double *) malloc(num_lineas *sizeof(double));
  fxx  = (double *) malloc(num_lineas *sizeof(double));
  

  for(i=0; i<num_lineas; i++)
    {
      nread = fscanf(pf1,"%lf %lf",&r_medio[i], &fxx[i]);
      fx[i] = r_medio[i] * fxx[i];
      
    }
  
    
   n= num_lineas ;
   
   h=(b-a)/(1.0*n);
   
   par = 0.0;
   for(i=1;i<(n/2-1);i++)
     {
      par +=  fx[2*i];
     }
   
   imp = 0.0;
   for(i=1;i<(n/2);i++)
     {
      imp += fx[2*i-1];
     }
   
   integ = (h*denominador)*(fx[0]+2*par+4*imp+fx[n]);
   
      
   return integ;

 
}


