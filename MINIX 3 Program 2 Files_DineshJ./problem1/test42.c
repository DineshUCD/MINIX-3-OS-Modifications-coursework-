#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>

int count;
void alarm_action(int);

main()
{
  int p[2];
  int pipe_size;
  char c = 'x';
  static struct sigaction act;

  /* setup signal handler */
  act.sa_handler = alarm_action;
  sigfillset(&(act.sa_mask));

  /* create the pipe */
  if(pipe(p) == -1)
  {
    perror("pipe call");
    exit(1);
  }
  
  /* determine the size of the pipe */
  pipe_size = fpathconf(p[0], _PC_PIPE_BUF);
  printf("Maximun size of write to pipe: %d bytes\n", pipe_size);

  sigaction(SIGALRM, &act, NULL);

  while(1)
  {
    /* set alarm */
    alarm(20);
    
    /* write down pipe */
    write(p[1], &c, 1);

    /* reset alarm */   
    alarm(0);
    
    if((++count % 1024) == 0)
      printf("%d characters in the pipe\n", count);
  }
}

/* called when SIGALRM received */
void alarm_action(int signo)
{
  printf("write blocked after %d characters\n", count);
  exit(0);
}
