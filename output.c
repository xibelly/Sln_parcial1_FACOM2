int write_file(char filename[], int Nparticles, struct particle *part, double *dist)
{
  int i;

  FILE *pf = NULL;
  
  pf = fopen(filename,"w"); 
  
  for(i=0; i<Nparticles; i++)
    {
      fprintf(pf,"%lf %lf %lf\n", part[i].pos[0], part[i].pos[1], dist[i]);
    }

  //fclose(pf);
}
