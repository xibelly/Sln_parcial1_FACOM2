#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#define NOM_ARCHIVO "inter_densidad.dat"
#define denominador 0.333333333

int simpson(double *r_medio, double *drho, int Nbins)
{
  int i, j, l, k, n, N, nread;
  int ch, num_lineas;
  double Iter_drho, *iter;
  double M, *M_tot;
  double Ri,*ri;
  double h; 
  double *I, *P;
  double S_a, S_b;

  FILE *pf = NULL;
  FILE *pf1 = NULL;
  FILE *pf2 = NULL;
  FILE *entrada =NULL;
  
  

  pf = fopen("inter_densidad.dat","w");

  /////////////////////////////INTERPOLACION -rho(r_medio)-/////////////////////
  gsl_interp_accel *acc 
    = gsl_interp_accel_alloc ();//iterador para la busqueda de interpolacion
  gsl_spline *spline 
    = gsl_spline_alloc (gsl_interp_akima, Nbins); //objeto de interpolacion de tipo akima
  
  gsl_spline_init (spline, r_medio, drho, Nbins);//inicializa el objeto de interpolacion
  
  for(Ri = r_medio[0]; Ri < r_medio[Nbins-1]; Ri=Ri+Ri)
    {
      Iter_drho = gsl_spline_eval (spline, Ri, acc);//se evalua la interpolacion
      fprintf(pf,"%g %g\n",Ri, Iter_drho);
    }
    
  gsl_spline_free (spline);
  gsl_interp_accel_free (acc);
  
  fclose(pf);

    ///////////////////////////INTEGRACION///////////////////////

  n = 8; //subintervals

  h = ( r_medio[Nbins] - r_medio[0] ) / (n); //ancho del intervalo

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
 
  ri  = (double *) malloc(num_lineas *sizeof(double));

  iter  = (double *) malloc(num_lineas *sizeof(double));

  M_tot = (double *) malloc(num_lineas *sizeof(double));
  
  pf1 = fopen("inter_densidad.dat", "r");

  for(i=0; i<num_lineas; i++)
    {
      nread = fscanf(pf1,"%lf %lf",&ri[i], &iter[i]);
      
    }

  pf2 = fopen("masa_integrada.dat","w");//integracion por el metdo de simpson 

  S_a = iter[0];
  
  S_b = iter[num_lineas];

  N = (int) floor(num_lineas*0.5);

  I  = (double *) malloc(N *sizeof(double));

  P =  (double *) malloc(N *sizeof(double));


  for(i=0; i<N; i++) //impar
    {
      j = 2*i +1;
      
      I[j] =  iter[j];
      
      
    }

  for(i=0; i<N; i++) //par
    {
      j = 2*i;
      
      P[j] =  iter[j];
      
    }

  for(i=0; i<num_lineas; i++)//regla de simpson compuesta
    {
      for(k=0; k<N; k++)
	{
	  j = 2*k +1;
	  
	  l = 2*k;
	  
	  M_tot[i] = (h * denominador) * ( S_a + (4 * I[j]) + (2 * P[l]) + S_b  );
	}
      fprintf(pf2,"%e %e\n",M_tot[i], ri[i]);
    }
      printf("RULE SIMPSON: SUCESS\n");
  
  //fclose(entrada);
  //fclose(pf);
  //fclose(pf1);
  //fclose(pf2);
  
  return 0;
}
