#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int create_forks(int num)
{
  static count = 0; 

  if(num < 0)
    return(0);

       sleep(5);
   switch(fork())
   {
     case -1:
       perror("fork call");
     case 0:
       count++;
       printf("Child #%d, pid #%d\n", count, getpid());
       create_forks(num - 1);
     default:
       for( ; ; )
       {
       }
    }

  exit(0);
}

main()
{
  int number_of_forks = 0;
  char c;

  do {
    printf("Enter number of forks: ");
  } while((scanf("%d%c", &number_of_forks, &c) != 1)  && number_of_forks <= 0);

  printf("Number of forks: %d\n", number_of_forks);

  create_forks(number_of_forks);

  exit(0);
}
