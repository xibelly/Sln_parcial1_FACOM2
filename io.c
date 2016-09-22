int read_file(char *filename, int Nparticles)
{
  int i;

  FILE *pf = NULL;
  
  pf = fopen(filename,"r"); 
  
  part = (struct particle *) malloc(Nparticles *sizeof(struct particle));

  printf(" ->In read_file:\n");
    
  for(i=0; i<Nparticles; i++)//lectura
    {
      fscanf(pf,"%lf %lf %lf",&part[i].[0], &part[i].[1]);
    }

  flcose(pf);

  
}
