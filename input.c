/*Xibelly Eliseth Mosquera Escobar
 * 
 * programa: input.c
 * 
 * 
 * Se encarga de leer los datos de
 * un archivo dado le longitud arbitraria
 * usando malloc.
 * 
 * 
 * La funcion "read_file" recibe como entradas
 * la longitud del archivo y el nombre del archivo
 * a leer. Ambas entradas se hacen usando 
 * la variable ARGS en el makefile incluido con
 * el programa.
 *
 * 
 
 */



int read_file(char *filename, int Nparticles)
{
  int i, nread;
  double X, Y;

  FILE *pf = NULL;
  
  pf = fopen(filename,"r"); 
  
  part = (struct particle *) malloc(Nparticles *sizeof(struct particle));

  printf(" ->In read_file:\n");
    
  for(i=0; i<Nparticles; i++)
    {
      nread = fscanf(pf,"%lf %lf",&X, &Y);

      part[i].pos[0] = X ; 

      part[i].pos[1] = Y ;
      
    }

  

  printf("READ FILE STATE: SUCESSFUL\n");
  
  return 0;
}
