// Description: This file works for downloading the data from wikimedia traffic data by using MPI
//mpicc -g -o getWiki_data getWiki_data.c
//mpiexec -f hosts -n 17 ./getWiki_data

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

int nnodes;
int me; // node's rank

void init(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nnodes);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  // n = strtol(argv[1], NULL, 0);

  // if (me == 0)
  // {
  //   int i;
  //   for (i = 0; i<n; i++)
  //   {
  //     list[i] = rand() % 6;
  //     printf("list[i]: %d\n", list[i]);
  //   }
  // }
}

void managernode()
{
  MPI_Status status;
  int i;

  //dividing data among all the nodes except managernode
  int sub_size = 584/(nnodes-1);
  int last_sub_size = sub_size;
  if (584 - (sub_size*(nnodes - 1)) != 0)
  {
    last_sub_size = sub_size +  (584 - sub_size*(nnodes - 1));
  }

  // send share of the downloading dates to worker nodes
  for (i = 1; i < nnodes-1; i++)
  {
    MPI_send (&sub_size, 1, MPI_INT, i, DATA_MSG, MPI_COMM_WORLD);
  }
  MPI_send (&last_sub_size, 1, MPI_INT, i, DATA_MSG, MPI_COMM_WORLD);
}

void workernode()
{
  int sub_size; // a buffer that receives the data
  MPI_Status status;

  //reeive message from manager node
  MPI_Recv(&sub_size, 1, MPI_INT, 0, DATA_MSG, MPI_COMM_WORLD, &status);

  //do work
  /*
  write
  code
  from
  here
  */
  // float sub_pi = calculate_pi(sub_size);

  //send result back to manager node
  // MPI_Send(&sub_pi, 1, MPI_INT, 0, NEWDATA_MSG, MPI_COMM_WORLD);
}



int main() {
    //start at 0153.11.9  --> 2007/12/09
    //2015   --> 0163.0.1
    //end at 0153.11.3171   -->2016/08/05
    char date[9];
    time_t t = time(NULL);
    struct tm * ptm = gmtime(&t);
    char year[4];
    char month[2];
    char nameOfGZ[26];
    char URL_forDay[82];
    char Download_gz_URL[100];
    char wgetCommand[154];
    char unZipCommand[114];
    char RemoveCommand[114];



    for (int i = 1; i < 584; i++)
    {
      ptm->tm_year = 0163;
      ptm->tm_mon = 0;
      ptm->tm_mday = i;
      mktime(ptm);
      strftime(date, 9, "%Y%m%d", ptm);

      memcpy(&year, &date[0], 4);
      year[4] = '\0';
      memcpy(&month, &date[4], 2);
      month[2] = '\0';

      snprintf(URL_forDay, 82, "https://dumps.wikimedia.org/other/pagecounts-raw/%s/%s-%s/pagecounts-%s", year, year, month, date);
      for (int j = 0; j < 24; j++)
      {
        if (j < 10)
        {
          snprintf(Download_gz_URL, 100, "%s-0%d0000.gz", URL_forDay, j);
        }
        else
        {
          snprintf(Download_gz_URL, 100, "%s-%d0000.gz", URL_forDay, j);
        }
        memcpy(&nameOfGZ, &Download_gz_URL[62], 26);
        nameOfGZ[26] = '\0';


        snprintf(wgetCommand, 154, "wget %s |hdfs dfs -put %s.gz ./WikiData/", Download_gz_URL, nameOfGZ);
        snprintf(unZipCommand, 114, "hadoop fs -text ./WikiData/%s.gz | hadoop fs -put - ./WikiData/%s", nameOfGZ, nameOfGZ);

        system(wgetCommand);
        system(unZipCommand);
      }
      system("hdfs dfs -rm ./Wikidata/*.gz");

    }
    return 0;
}
