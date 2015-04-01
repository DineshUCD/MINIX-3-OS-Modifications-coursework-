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

  for(;;)
  {
    oldmask = umask(022);
    pid = getpid(); 
    pipe(p);

    filedes = open(workfile, O_RDWR);
    for(inner1 = 0; inner1 < 100; inner1++)
      lseek(filedes, (off_t)0, SEEK_SET);
    nread = read(filedes, buffer, 1024);

    chmod(workfile, 0644);

    link(workfile, "new_workfile");


    unlink("new_workfile");
    close(filedes);

    sleep(1);
  }

  exit(0);
}

