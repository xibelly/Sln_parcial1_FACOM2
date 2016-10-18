/*
Xibelly Eliseth Mosquera Escobar

Solucion Parcial I COMPUTACION CIENTIFICA AVANZADA

Para solucionar el parcial, mas especificamnete para calcular los vecinos tenemos dos modos de operacion:

Modo1) Malla

Modo2) Fuerza bruta

En este caso haremos uso del modo2, el cual no es tan eficiente.

La solucion de los problemas viene dada de la siguiente forma:

1) Se cargan los datos

2) Se busca el centro geometrico, esto es, identificar la posicion de la particula mas cerca al centro (0,0)

3) Calculamos los vecinos e imponemos un conjunto de condiciones para identificar unicamente los 2 mas cercanos

4) Calculamos la distancia de cada triangulo al centro del sistema, tal que podamos hacer una grafica de  #triangulos vs distancia centro e imprimimos en disco

5) Calculamos la densidad superficial de masa S(x,y) a traves de un bineo e imprimimos en disco

6) Hacemos una interpolacion a los datos anteriores para obterner una funcion S(R) e imprimimos en disco para luego con esta interpolacion poder identificar el radio al cual la densidad ha decaido por debajo de e.

7) Hacemos uso de la interpolacion realizada para calcular la masa total del sistema y la masa acumulada a una radio del bin dado, por medio de la regla de Simpson compuesta.


 */


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include <gsl/gsl_sort_int.h>
#include <gsl/gsl_sort_double.h>

#include <distancia.h>
#include <interpolador.h>
#include <simpson.h>
#include <decae_densidad.h>

#define m 1
#define PI 3.141592


//////VARIABLE GLOBALES//////

clock_t tini, tend, tacum;
double cpu_time_used;

int Nparticles, part_bines;
double *dist, *dist_centro;
double X_centro, Y_centro;


FILE *pf6=NULL;
FILE *pf7=NULL;
FILE *pf8=NULL;
FILE *pf9=NULL;

/////USO DE ESTRUCTURAS////////

struct particle   
{
  double pos[2];
  double x, y;
  int id;
  double vertice[2];
  
}; 
struct particle *part; 

struct vecinos
{
  double pos1_x, pos1_y;
  double pos2_x, pos2_y;
  double abs_dist[2];
  int k;
}; 
struct vecinos *vec; 


///////////LLAMADOS A SUBPROGRAMAS///////////

#include"input.c"


/////////////PROGRAMA PRINCIPAL/////////////////////

int main(int argc, char **argv) 
{
  int  i, j, l, nread, n, nn;
  int suma;
  int Nbins;
  int No;
    
  char *filename;

  double distmin, distmax;
  double r_xi, r_yi, r_xj, r_yj;
  double xi, yi, xj, yj;
  double Xi, Yi, Xj, Yj;
  double x1, y1, x2, y2, x3, y3; 

  double *BX, *BY;

  double Rmax, Rmin;
  double dr;
  double ri, re, ri_exp, re_exp, *r_medio, *r_medio2;
  double dS, dSi, dSe;
  double particula,dM, *drho, *drho2;
  double d_prueba1, d_prueba2;
  double dis_euclidea;

 
  double Mtot, M;
  double dr2, ri2;
  
  printf("%d\n",argc);

  if(argc != 4)
    {
      printf("ERROR--> use as:\n");
      printf("%s Npartilces part_bines datafile\n",argv[0]);
      exit(0);  
    }

  //Carga de parametros//

  Nparticles = atoi(argv[1]);
  
  part_bines   =  atoi(argv[2]);

  filename   = argv[3];

  printf("%s %d %d %s\n",argv[0], Nparticles, part_bines, filename);

   
   
  dist  = (double *) malloc(Nparticles *sizeof(double));  //particulas
 
  
  part  = (struct particle *) malloc(Nparticles *sizeof(struct particle));

  vec  = (struct vecinos *) malloc(Nparticles *sizeof(struct vecinos));
  
  
  BX   = (double *) malloc(n* sizeof(double));//triangulos

  BY   = (double *) malloc(n* sizeof(double));
   

  size_t p[Nparticles];

  int k = Nparticles;


    
  
  //Carga de datos//
  
  read_file(filename, Nparticles);

  

  //Distancia  al centro del sistema //
  
    
  for(i=0; i<Nparticles; i++)
    {
      
      dist[i] = distancia(0, part[i].pos[0], 0, part[i].pos[1]);
    }
  
  gsl_sort_smallest_index (p, k, dist, 1,Nparticles);//Se ordenan las particulas en forma creciente
  
  free(dist);

  
  
  // Coordenadas del centro geometrico//

  X_centro =  part[p[0]].pos[0];

  Y_centro =  part[p[0]].pos[1];

  printf("CENTER COORDINATES\n");

  printf("Xc: %lf Yc: %lf\n", X_centro, Y_centro);


  
  
  //Trasladamos el sistema al centro (0,0)//

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

  gsl_sort_smallest_index (p, k, dist_centro, 1,Nparticles);//Se ordenan las particulas en forma creciente
  
  
  
  //Calculamos los vecinos -los 2 mas cercanos- //

  tini = clock();
  
  for(i=0; i<Nparticles; i++)
    {
      vec[i].pos1_x = 0.0;
      vec[i].pos1_y = 0.0;
      
      vec[i].pos2_x = 0.0;
      vec[i].pos2_y = 0.0;

      
      d_prueba1 = 1.0E10;
      d_prueba2 = 1.0E10;
      
      for(j=0; j<Nparticles; j++)
	{
	  if(j != i)
	    {
	      r_xi = part[i].pos[0];
	      r_yi = part[i].pos[1];
	      
	      r_xj = part[j].pos[0];
	      r_yj = part[j].pos[1];
	      
	      dis_euclidea = distancia(r_xi, r_xj, r_yi, r_yj);
	      
	      if(dis_euclidea < d_prueba1)
		{
		  d_prueba2 = d_prueba1;
		  d_prueba1 = dis_euclidea;

		  vec[i].pos1_x = part[j].pos[0];
		  vec[i].pos1_y = part[j].pos[1];
		  
		  vec[i].abs_dist[0] = dis_euclidea;
		}
	      
	      
	      else if(dis_euclidea > d_prueba1  && dis_euclidea < d_prueba2)
		{
		  vec[i].pos2_x = part[j].pos[0];
		  vec[i].pos2_y = part[j].pos[1];

		  d_prueba2 = dis_euclidea;
		  vec[i].abs_dist[1] = d_prueba2;
		}
	      
	    }
	  
	  
	}
      
      part[i].vertice[0] = part[i].pos[0];
      part[i].vertice[1] = part[i].pos[1];
      
    }
  
  pf6 = fopen("2vecinos_mas_cercanos.dat","a");
  
  for(i=0; i<Nparticles; i++)
    {
      fprintf(pf6,"---------------------\n\n"); 
      fprintf(pf6," NEIGHBORS OF THE PARTCILES %d AREA:\n\n", i); 
      
      fprintf(pf6,"DISTANCE TO THE NEIGHBORS\n");

      fprintf(pf6,"%f\n",  vec[i].abs_dist[0]);
      fprintf(pf6,"%f\n\n", vec[i].abs_dist[1]);
      
      fprintf(pf6,"CENTRAL PARTICLE\n"); 
      
      fprintf(pf6,"%f\n", part[i].vertice[0]);
      fprintf(pf6,"%f\n\n", part[i].vertice[1]);
      
      fprintf(pf6,"NEIGHBOR 1\n"); 
      
      fprintf(pf6,"%f\n", vec[i].pos1_x);
      fprintf(pf6,"%f\n\n", vec[i].pos1_y);
      
      fprintf(pf6,"NEIGHBOR 2\n"); 
      
      fprintf(pf6,"%f\n", vec[i].pos2_x);
      fprintf(pf6,"%f\n\n", vec[i].pos2_y);
      
    }
  
  tend = clock();
  
  cpu_time_used = ((double) (tend - tini)) / CLOCKS_PER_SEC;
  
  printf("CPU TIME USED TO FIND THE TWO NEAREST NEIGHBORS: %g\n",cpu_time_used);
  

  
    
  //Calculamos las distancias cada triangulo al centro (0,0)//
  
  
  
  pf7 = fopen("triangulos-centro.dat","w");

  suma = 0;
  
  for(i=0; i<Nparticles; i++)
    {
      x1 = part[i].vertice[0];
      y1 = part[i].vertice[1];
      
      for(j=0; j<Nparticles; j++)
	{
	  x2 = vec[j].pos1_x;
	  y2 = vec[j].pos1_y;

	  x3 = vec[j].pos2_x;
	  y3 = vec[j].pos2_y;
	  	  
	  
	  if( x1 == x2 || x1 == x3)
	    {
	      vec[i].k +=1;
	      
	      
	    }

	  
	}

    }

  for(i=0; i<Nparticles; i++)
    {
      if(vec[p[i]].k != 0)
      	fprintf(pf7,"%d %lf\n",vec[p[i]].k,dist_centro[p[i]]);
    }
  
  
  
  printf("SISTEM TRIANGLES-CENTER: SUCESS\n");
  
  
  
  //Densidad superficial de masa -BINEO- //

 
  Rmax = distmax;
  Rmin = 0.01*Rmax;
  Nbins = floor(Nparticles/part_bines);  
  
  dr = (Rmax-Rmin)/Nbins;
   
  particula =  part_bines;

  int r1, r2;

  size_t p2[Nbins];

  int k2 = Nbins;

    
  r_medio  = (double *) malloc(Nbins *sizeof(double));
  
  drho  = (double *) malloc(Nbins *sizeof(double));

  r_medio2  = (double *) malloc(Nbins *sizeof(double));
  
  drho2  = (double *) malloc(Nbins *sizeof(double));

  

  
  pf8 = fopen("densidad_distancia.dat","w");

  
  for(i=0; i<Nbins; i++)
    {
      r1 = i * part_bines;
      r2 = r1 + (part_bines -1);
      
      ri =dist_centro[p[r1]]; 
      re =dist_centro[p[r2]] ;


      r_medio[i] = (re - ri)*0.5;
      r_medio[i] = sqrt( r_medio[i] * r_medio[i]);
	  
      dSi = (PI*ri*ri); 
      dSe = (PI*re*re);
	  
      dS = dSe - dSi;
      dS = sqrt(dS * dS);
      
      
      dM = part_bines*m;
      
      drho[i] = dM/dS;
	  
      	  
    }

  gsl_sort_smallest_index (p2, k2, r_medio, 1,Nbins);//organizamos en forma creciente los radios de los bines
  
  for(i=0; i<Nbins; i++)
    {
         
      r_medio2[i] = r_medio[p2[i]];
      drho2[i] = drho[p2[i]];
      
      fprintf(pf8,"%e %e\n",r_medio2[i],drho2[i]);
      
      }

    printf("SISTEM DENSITY-DISTANCE: SUCESS\n");
  

  
  //Interpolacion densidad - Calculo Masa total//
  
  Mtot = interpolador(r_medio2, drho2, Nbins, r_medio[Nbins-1]);
 
  printf("TOTAL MASS IS: %e\n",Mtot);
  
  //Calculo de la Masa acumulada//

  
  pf9 = fopen("Masa_Acumulada.dat","w");
  No = 100;
  dr2 = (r_medio2[Nbins-1]-r_medio2[0])/(1.0*No);
  for(i=0;i<No;i++)
    {
      ri = r_medio2[0]+i*dr2;
      M = interpolador(r_medio2, drho2, Nbins, ri);
      fprintf(pf9,"%lf %lf \n", ri, M);
    }
  

  printf("STATE INTEGRATION: SUCESS\n");
  
  
  
  //Identificacion del radio al cual la densidad decae por debajo de e//
      
   
  decae_densidad();

  printf("STATE RADIUS WHERE THE DENSITY DECAES BY A FACTOR OF e: SUCESS\n");

    
  
      
  //Se libera la memoria usada//
 
  free(part);
  free(vec);
      
  fclose(pf6);
  fclose(pf7);
  fclose(pf8);
  fclose(pf9);
  
  
  
  return 0;
  
}

