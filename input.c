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

      part[i].pos[0] = X + 1500; //re-escalamos el sistema

      part[i].pos[1] = Y + 1500;
      
    }

  //flcose(pf);

  printf("READ FILE STATE: SUCESSFUL\n");
  
  return 0;
}
