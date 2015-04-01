#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>

#define BUFSIZE 512  /* size of chunk to be read */
#define PERM    0644 /* file permission for new file */
#define TRUE    1
#define FALSE   0

int copyfile( const char *name1, const char *name2)
{
  int infile, outfile;
  ssize_t nread;
  char buffer[BUFSIZE];
  int repeat = 0;

  if( (infile = open(name1, O_RDONLY) ) == -1)
    return(-1);

  if( (outfile = open(name2, O_WRONLY|O_CREAT|O_TRUNC, PERM)) == -1) {
    close(infile);
    return(-2);
  }
   
  while(TRUE)
  {
    if((nread = read(infile, buffer, BUFSIZE)) == 0) 
    {
      lseek(infile, (off_t)0, SEEK_SET);
      lseek(outfile, (off_t)0, SEEK_SET);
      ++repeat;
    }

    if(repeat > 10) break; 


    if( write(outfile, buffer, nread) < nread)
    { 
      close(infile);
      close(outfile);
      return(-3);  /* write error */
    }
    sleep(1);
  }

  close(infile);
  close(outfile);

  if (nread == -1) /* error on last read */
    return(-4);
  else
    return(0);
}

main()
{
  copyfile("test.in", "test.out");
}

