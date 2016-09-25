/*
Xibelly Eliseth Mosquera Escobar

Solucion Parcial I COMPUTACION CIENTIFICA AVANZADA

Para solucionar el parcial, mas especificamnete para calcular los vecinos tenemos dos modos de operacion:

Modo1) Malla

Modo2) Fuerza bruta


 */


/*
 Parametros ideales:

Lbox = 3000

Nbox = 25

 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include <gsl/gsl_sort_int.h>
#include <gsl/gsl_sort_double.h>

#include <distancia.h>
#include <simpson.h>
#include <decae_densidad.h>

#define m 1
#define PI 3.141592


//////VARIABLE GLOBALES//////

clock_t tini, tend, tacum;
double cpu_time_used;

int Nparticles;
double *dist, *dist_centro;
double X_centro, Y_centro;

FILE *pf2=NULL;
FILE *pf3=NULL;
FILE *pf4=NULL;
FILE *pf5=NULL;
FILE *pf6=NULL;
FILE *pf7=NULL;
FILE *pf8=NULL;

struct particle   
{
  double pos[2];
  double x, y;
  int id;
  
}; 
struct particle *part; 

struct vecinos
{
  int *id_i, *id_j;
  double *pos_x, *pos_y;
  double *R;
  double *XX, *YY;
}; 
struct vecinos vec; 


///////////LLAMADOS A fUNCIONES///////////

#include"input.c"


//////////////////////////////////

int main(int argc, char **argv) 
{
  int  i, j, l, nread, n, tamano, *ID;
  int suma;
  int Nbins;
  int part_bines;
    
  char *filename;

  double distmin, distmax;
  double r_xi, r_yi, r_xj, r_yj;
  double xi, yi, xj, yj;
  double Xi, Yi, Xj, Yj;
  double x1, y1, x2, y2, x3, y3; 

  double *BX, *BY;

  double Rmax, Rmin;
  double dr;
  double ri, re, ri_exp, re_exp, *r_medio;
  double dS, dSi, dSe;
  double particula, dM, *drho;
  
  printf("%d\n",argc);

  if(argc != 4)
    {
      printf("ERROR--> use as:\n");
      printf("%s Npartilces part_bines datafile\n",argv[0]);
      exit(0);  
    }

  /*Carga de parametros*/

  Nparticles = atoi(argv[1]);
  
  part_bines   =  atoi(argv[2]);

  filename   = argv[3];

  printf("%s %d %d %s\n",argv[0], Nparticles, part_bines, filename);

  n     = Nparticles * Nparticles;
  
  tamano = Nparticles-1;
  
  dist  = (double *) malloc(Nparticles *sizeof(double));  //particulas
 
  ID  = (int *) malloc(Nparticles *sizeof(int));  

  //drho  = (double *) malloc(Nparticles *sizeof(double));


  part  = (struct particle *) malloc(Nparticles *sizeof(struct particle));

  vec.pos_x = (double *) malloc( n *sizeof(double));  //vecinos

  vec.pos_y = (double *) malloc( n *sizeof(double));
  
  vec.R    = (double *) malloc( n *sizeof(double));

  vec.id_i = (int *) malloc( n *sizeof(int));

  vec.id_j = (int *) malloc( n *sizeof(int));
  
  vec.XX = (double *) malloc( n *sizeof(double));

  vec.YY = (double *) malloc( n *sizeof(double));
  
  BX   = (double *) malloc(n* sizeof(double));//triangulos

  BY   = (double *) malloc(n *sizeof(double));
   

  size_t p[Nparticles];

  int k = Nparticles;

    
  
  /*Carga de datos*/
  
  read_file(filename, Nparticles);

  //pf2 = fopen("coordenadas2.dat","w");

  /*for(i=0; i<Nparticles; i++)//lectura
    {
      part[i].id = i;
      //fprintf(pf2,"%d %lf %lf\n",part[i].id, part[i].pos[0], part[i].pos[1]);
      }*/

  
  
  /*Calculamos los vecinos -los 2 mas cercanos- */

  tini = clock();
  
  for(i=0; i<Nparticles; i++)
    {
        for(j=0; j<Nparticles; j++)
	  {
	    if(j != i)
	      {
		r_xi = part[i].pos[0];
		r_yi = part[i].pos[1];

		r_xj = part[j].pos[0];
		r_yj = part[j].pos[1];

		dist[j] = distancia(r_xi, r_xj, r_yi, r_yj);
		//dist[j] = sqrt( ((r_xi - r_xj)*(r_xi - r_xj)) + ((r_yi - r_yj)*(r_yi - r_yj)) );
		//printf("%lf\n", dist[j]);

	      }
	    part[i].id = i;
	    
	  }
	
    }

  tend = clock();
  
  cpu_time_used = ((double) (tend - tini)) / CLOCKS_PER_SEC;

  printf("CPU TIME USED TO FIND NEIGHBORS: %g\n",cpu_time_used);
  
  gsl_sort_smallest_index (p, k, dist, 1, Nparticles);//Se ordenan las particulas en forma creciente

 
  pf3 = fopen("vecinos.dat","w");
  
  for(i=0; i<Nparticles; i++)
    {
      for(j=0; j<Nparticles; j++)
	  {
	    if(j != i)
	      {
		fprintf(pf3,"%d %lf %lf %d %lf %lf %lf\n", part[i].id, part[i].pos[0], part[i].pos[1], part[p[j]].id, part[p[j]].pos[0], part[p[j]].pos[1], dist[p[j]]);

	      }
	  }
    }
  fclose(pf3);
  
  pf4 = fopen("vecinos.dat","r");

  for(i=0; i<n; i++)
    {
      nread = fscanf(pf4,"%d %lf %lf %d %lf %lf %lf", &vec.id_i[i], &vec.XX[i], &vec.YY[i],&vec.id_j[i], &vec.pos_x[i], &vec.pos_y[i], &vec.R[i]);
    }
  
  pf5 = fopen("2vecinos_mas_cercanos.dat","a");

  for(i=0; i<n; i=i+Nparticles)//dos vecinos mas cercanos
    {
      fprintf(pf5,"PARTICLE %d HAS 1 TRIANGLE\n",vec.id_i[i]);
      fprintf(pf5,"THEIR TOW NEIGHBORS ARE: %d  %d\n", vec.id_j[i], vec.id_j[i+1]);  
      fprintf(pf5,"WITH COORDINATES:\n");
      fprintf(pf5,"x: %lf y: %lf x: %lf y: %lf\n",vec.pos_x[i], vec.pos_y[i], vec.pos_x[i+1], vec.pos_y[i+1]);
      

    }
    
  //free(dist);
    
   
  /*Distancia  al centro del sistema */
  
    
  for(i=0; i<Nparticles; i++)
    {
      //dist[i] = distancia(1500, part[i].pos[0], 1500, part[i].pos[1]);
      dist[i] = distancia(0, part[i].pos[0], 0, part[i].pos[1]);
    }
  
  gsl_sort_smallest_index (p, k, dist, 1,Nparticles);//Se ordenan las particulas en forma creciente
  
  free(dist);
  
  
  /* Coordenadas del centro geometrico*/

  X_centro =  part[p[0]].pos[0];

  Y_centro =  part[p[0]].pos[1];

  printf("CENTER COORDINATES\n");

  printf("Xc: %lf Yc: %lf\n", X_centro, Y_centro);
  
  
  /*Trasladamos el sistema al centro (0,0)*/

  dist_centro = (double *) malloc(Nparticles *sizeof(double));

  distmin = 100*Nparticles;

  distmax = 0.0;
  
  for(i=0; i<Nparticles; i++)
    {
      xi = X_centro - X_centro;
      yi = Y_centro - Y_centro;

      xj =  part[i].pos[0] - X_centro;
      yj =  part[i].pos[1] - Y_centro;
      
      dist_centro[i] = distancia(xi, xj, yi, yj); //distancia de las particulas al nuevo centro
      
      if(distmin>dist_centro[i])//se calcula la distancia min y max
	{
	  distmin=dist_centro[i];
	}
      
      
      if(dist_centro[i]>distmax)
	{
	  distmax=dist_centro[i];
	}
    }
  printf("MIN VALUE OF DIST %.9lf\n",distmin);
  printf("MAX VALUE OF DIST: %.9lf\n",distmax);

   

  
  /*Calculamos las distancias de los baricentros de cada triangulo al centro (0,0)*/
  
  dist = (double *) malloc(n *sizeof(double));

  pf6 = fopen("baricentros.dat","w");
  pf7 = fopen("triangulos-centro.dat","w");
  

  for(i=0; i<n; i++)//calculo del baricentro
    {
      x1 = vec.XX[i]; //coordenadas particula principal
      y1 = vec.YY[i];

      x2 = vec.pos_x[i];//coordenadas vecinas que forman el triangulo
      y2 = vec.pos_y[i];
      
      x3 = vec.pos_x[i+1];
      y3 = vec.pos_y[i+1];
      
      BX[i] = (x1 + x2 + x3)*0.3; //coordenadas baricentros
      
      BY[i] = (y1 + y2 + y3)*0.3;

      fprintf(pf6,"%lf %lf\n", BX[i], BY[i]);
	  

    }

  for(j=0; j<n; j++)//distancia baricentros al centro del sistema
    {
      Xi = X_centro - X_centro;
      Yi = Y_centro - Y_centro;

      Xj = BX[j] - X_centro;
      Yj = BY[j] - Y_centro;

       
      dist[j] = distancia(Xi, Xj, Yi, Yj); //distancia de las particulas al nuevo centro
      
      fprintf(pf7,"%d %lf\n",j, dist[j]);
	
      }

  /*Densidad superficial de masa -BINEO- */

  Rmax = distmax;
  Rmin = 0.01*Rmax;
  Nbins = 1*sqrt(Nparticles);  
  dr = part_bines * ( log(Rmax)-log(Rmin) )/Nbins;

  r_medio  = (double *) malloc(Nbins *sizeof(double));

  drho  = (double *) malloc(Nbins *sizeof(double));

  pf8 = fopen("densidad_distancia.dat","w");
  
  for(l=0; l<Nbins; l++)
    {
      ri =log(Rmin)+ l * dr ; 
      re = ri + dr ;
               
      ri_exp = exp(ri);
      re_exp = exp(re);

      r_medio[l] = (re_exp - ri_exp)*0.5;

      dSi = (2*PI*ri_exp); 
      dSe = (2*PI*re_exp);
      
      dS = dSe - dSi;
      
      particula = 0.0;
      for(i=0; i<Nparticles; i++)
	{
	  if(dist_centro[i] >= ri_exp && dist_centro[i] <= re_exp)
	    {
	      particula++;
	      dM = particula*m;
	    }

	}
      

      drho[l] = dM/dS;

      fprintf(pf8,"%e %e\n",r_medio[l], drho[l]);

    }
  
  /*Calculo de la Masa Integrada*/

  simpson(r_medio, drho, Nbins);

  /*Identificacion del radio al cual la densidad decae por debajo de e*/

  decae_densidad();
  
  /* Imprimir resultados y salir */
  
 
  //fclose(pf2);
  fclose(pf4);
  fclose(pf5);
  fclose(pf6);
  fclose(pf7);
  fclose(pf8);
  return 0;

}
