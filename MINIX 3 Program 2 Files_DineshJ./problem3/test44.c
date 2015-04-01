#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

char *workfile="test.in";
main()
{
  int infinite = 0;
  int inner1   = 0;
  pid_t pid;
  ssize_t nread;
  char buffer[1024];
  int filedes;
  int p[2];
  mode_t oldmask;

  oldmask = umask(022);
  
  sleep(5); 
  pid = getpid(); 
  pipe(p);

  filedes = open(workfile, O_RDWR);
  for(inner1 = 0; inner1 < 100; inner1++)
    lseek(filedes, (off_t)0, SEEK_SET);
  nread = read(filedes, buffer, 1024);

  sleep(5);

  chmod(workfile, 0644);

  sleep(5);

  link(workfile, "new_workfile");

  sleep(3);

  switch(fork())
  {
    case -1:
      printf("Fork failed!");
      break;
    case 0:
      sleep(3);
      break;
    default:
      wait(NULL);
  }

  sleep(5);

  unlink("new_workfile");
  close(filedes);

  sleep(10);

  printf("PIPE #42  = 1, ALARM #27 > 7, OPEN #5 = 1, READ #3 = 1\n");
  printf("WAIT #7   = 7, LINK #9  = 1, LSEEK #19 = 100, CHMOD #15 = 1 \n");
  printf("UMASK #60 = 1, FORK = 2, UNLINK #10 = 1,  GETPID #20 = 1\n");   
  printf("CLOSE #6 = 1\n");

  exit(0);
}

