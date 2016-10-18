/*Xibelly Eliseth Mosquera Escobar
 * 
 * programa: interpolador.c
 * 
 * 
 * Se encarga de interpolar los datos de densidad superficial-distancia centro
 * sistema, los cuales se usan tanto para hacer la integracion a traves de la
 * regla de simpson y determinar la masa total del sistema y la masa acumulada. 
 * Asu vez es usado para poder identificar para que valor del radio la densidad
 * decae por un fator de e
 *  
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include"simpson.h"


int interpolador(double *r_medio, double *drho, int Nbins, double b)
{
  
  double Iter_drho, *iter;
  double Ri;
  double Mr;
  double inter;

  FILE *pf = NULL;
 

     
  pf = fopen("inter_densidad.dat","w");

   /////////////////////////////INTERPOLACION -rho(r_medio)-/////////////////////
  gsl_interp_accel *acc 
    = gsl_interp_accel_alloc ();//iterador para la busqueda de interpolacion
  gsl_spline *spline 
    = gsl_spline_alloc (gsl_interp_akima, Nbins); //objeto de interpolacion de tipo akima

   gsl_spline_init (spline, r_medio, drho, Nbins);//inicializa el objeto de interpolacion
 
  
  for(Ri = r_medio[0]; Ri < b; Ri=Ri+0.1)
    {
      Iter_drho = gsl_spline_eval (spline, Ri, acc);//se evalua la interpolacion
      inter = Iter_drho*Ri;
      fprintf(pf,"%g %g\n",Ri,inter);
    }
    
  gsl_spline_free (spline);
  gsl_interp_accel_free (acc);
  
  fclose(pf);

  Mr = simpson(r_medio[0], b);
  Mr *= 2*M_PI;
  
  return Mr;
}

