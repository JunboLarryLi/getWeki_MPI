// Description: This file is used for downloading missing dates. (NO MPI INVOLVED HERE)
//gcc CURATE_DAY.c -o CURATE_DAY -std=c99
//./CURATE_DAY

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int cmd;

void renameFiles(char * fileName)
{
  FILE * fp;
  char line[80];
  fp = fopen(fileName, "r");
  while (fgets(line, sizeof(line), fp)) {
      cmd = system(line);
  }
  fclose(fp);
}

int main(int argc, char** argv)
{
  if (argc != 2) {
    exit(-1);
  } else {
  renameFiles(argv[1]);
  }
  return 0;
}
