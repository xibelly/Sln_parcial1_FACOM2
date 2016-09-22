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

//////VARIABLE GLOBALES//////

clock_t tini, tend, tacum;
double cpu_time_used;

int Nparticles;
double *dist;

///////intento


FILE *pf2=NULL;
FILE *pf3=NULL;
FILE *pf4=NULL;

struct particle   
{
  double pos[2];
  double x, y;
  int id;
  
}; 
struct particle *part; 


////////////////////////////////////

///////////LLAMADOS A fUNCIONES///////////

#include"input.c"

#include"output.c"

//////////////////////////////////

int main(int argc, char **argv) 
{
  int  i, j, nread, c, id_i, id_j;
  int suma;
  double a, b;

  
  char *filename;

  double distmin, distmax;
  double X_centro, Y_centro;
  double r_xi, r_yi, r_xj, r_yj;
  double xi, yi, xj, yj;
  double R;
  

  printf("%d\n",argc);

  if(argc != 3)
    {
      printf("ERROR--> use as:\n");
      printf("%s Npartilces datafile\n",argv[0]);
      exit(0);  
    }

  /*Carga de parametros*/

  Nparticles = atoi(argv[1]);
  
  filename   = argv[2];

  printf("%s %d %s\n",argv[0], Nparticles, filename);

  
  
  dist = (double *) malloc(Nparticles *sizeof(double));  //particulas

  part = (struct particle *) malloc(Nparticles *sizeof(struct particle));

  //part.id = (int *) malloc(Nparticles *sizeof(int));
  

  size_t p[Nparticles];

  int k = Nparticles;

  
  /*Carga de datos*/
  
  read_file(filename, Nparticles);

  //pf2 = fopen("coordenadas2.dat","w");

  for(i=0; i<Nparticles; i++)//lectura
    {
      //fprintf(pf2,"%lf %lf\n",part[i].pos[0], part[i].pos[1]);
    }

  
  
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

	      }
	    part[i].id = i;

	  }
	
    }

  tend = clock();
  
  cpu_time_used = ((double) (tend - tini)) / CLOCKS_PER_SEC;

  printf("CPU TIME USED TO FIND NEIGHBORS: %g\n",cpu_time_used);
  
  gsl_sort_smallest_index (p, k, dist, 1, Nparticles);//Se ordenan las particulas en forma creciente

  
  //pf3 = fopen("vecinos.dat","w");
  
  for(i=0; i<Nparticles; i++)
    {
      for(j=0; j<Nparticles; j++)
	  {

	    //fprintf(pf3,"%d %d %lf %lf %lf\n", part[i].id, part[p[j]].id, part[p[j]].pos[0], part[p[j]].pos[1], dist[p[j]]);
	  }
    }
  fclose(pf3);
  
  pf4 = fopen("vecinos.dat","r");

  for(i=0; i<Nparticles*Nparticles; i++)
    {
      nread = fscanf(pf4,"%d %d %lf %lf %lf", &id_i, &id_j, &part[i].x, &part[i].y, &R);

      c = i+Nparticles;
      while(c)
	{
	  printf("PARTICLE %d HAS 1 TRIANGLES\n",part[i].id);
	  printf("WITH COORDINATES: \n");  
	  printf("%lf %lf\n",part[c].x, part[c].y);
	}
    }
  
  /*for(i=0; i<Nparticles*; i++) //dos vecinos mas cercanos
    {
      for(j=0; j<Nparticles; j++)
	  {
	    //c =1* (j * Nparticles);
	    printf("%d\n",c);
	    printf("PARTICLE %d HAS 1 TRIANGLES\n",part[i].id);
	    printf("WITH COORDINATES: \n");  
	    printf("%lf %lf\n",part[p[j]].pos[0], part[p[j]].pos[1]);
          }
	
	  }*/
  
  free(dist);
    
  exit(0);
  
  /*Distancia  al centro del sistema */

  
  for(i=0; i<Nparticles; i++)
    {
      dist[i] = distancia(1500, part[i].pos[0], 1500, part[i].pos[1]);
    }
  
  gsl_sort_smallest_index (p, k, dist, 1,Nparticles);//Se ordenan las particulas en forma creciente
  
  free(dist);
  
  
  /* Coordenadas del centro geometrico*/

  X_centro =  part[p[0]].pos[0];

  Y_centro =  part[p[0]].pos[1];

  printf("CENTER COORDINATES\n");

  printf("Xc: %lf Yc: %lf\n", X_centro, Y_centro);
  
  
  /*Trasladamos el sistema al centro (0,0)*/

  dist = (double *) malloc(Nparticles *sizeof(double));

  distmin = 100*Nparticles;

  distmax = 0.0;
  
  for(i=0; i<Nparticles; i++)
    {
      xi = X_centro - X_centro;
      yi = Y_centro - Y_centro;

      xj =  part[i].pos[0] - X_centro;
      yj =  part[i].pos[1] - Y_centro;
      
      dist[i] = distancia(xi, xj, yi, yj); //distancia de las particulas al nuevo centro
      
      if(distmin>dist[i])//se calcula la distancia min y max
	{
	  distmin=dist[i];
	}
      
      
      if(dist[i]>distmax)
	{
	  distmax=dist[i];
	}

      //write_file("distancia_centro.dat", Nparticles, part, dist);
    }
  printf("MIN VALUE OF DIST %.9lf\n",distmin);
  printf("MAX VALUE OF DIST: %.9lf\n",distmax);
 
  
  
  
  
  /* Imprimir resultados y salir */


  //fclose(pf2);
  
  return 0;

}
