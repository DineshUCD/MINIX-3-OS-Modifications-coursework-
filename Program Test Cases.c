//Dinesh Jayasankar
//ID: 997960868
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <math.h>

/*	Problem 1	*/
ssize_t readfile(int filedes, int evenOrOdd) 
{
  char temp[1];
  ssize_t nread;
  unsigned int lineNumber = 1;

  while((nread = read(filedes, temp, (size_t)1)) > 0)
  {
    if(temp[0] != '\n')
    {
      if(lineNumber % 2 == evenOrOdd)
        printf("%c", temp[0]);
    }
    else 
    {
      if(lineNumber % 2 == evenOrOdd)
        printf("\n"); 
      lineNumber++;
    }
  }
  return nread;
} //readfile()  

int Odds_then_evens(char *path)
{
  int fd;
  off_t newpos;
  ssize_t nread;
  int even = 0;
  int odd  = 1;
  if((fd = open(path, O_RDONLY)) == -1)
  {
    printf("Couldn't open %s\n", path);
    exit(1);
  }

  nread = readfile(fd, odd);

  if(nread == -1)  
    return(-1);

  if((newpos = lseek(fd, (off_t)0, SEEK_SET) == -1))
    return(-2);

  nread = readfile(fd, even);
    
  close(fd);

  if(nread == -1)
    return(-3);
  else
    return(0);
} //Odds_then-evens
/*------------------------------------------*/

/*	Problem 2	*/
int fatal(char *s)	/* print error message and die */
{
  perror(s);
  exit(1);
} //fatal()
 
int compare(char original[], char output[], int start_position)
{
  int compare = 0;
   while(original[compare] == output[start_position + compare])
   {
     if(original[compare] == '\0' || output[compare + start_position] == '\0')
       break;
     compare++;
   }
   if(original[compare] == '\0' && output[compare + start_position] == '\0')
     return(0);
   else
     return(-1);
}

int connect()
{
  int p_first_to_second[2], status_childOfOne,
      p_second_to_first[2];

  char buffer_process1[80], from_second_process1[160];
  ssize_t write_process1, read_process1;
  int start_compare = 0;

  if(pipe(p_first_to_second) == -1)
    fatal("pipe call in connect");

  if(pipe(p_second_to_first) == -1)
    fatal("pipe call in connect");

  switch(fork())
  {
    case -1:	/* error */
      fatal("1st fork call in connect");
    case 0:	/* child */
      break;
    default:	/* parent */

      close(p_first_to_second[0]);
      close(p_second_to_first[1]);

      printf("Please enter a string: ");
      scanf("%80[0-9a-zA-Z ]s", buffer_process1);
      printf("%s\n", buffer_process1);

      if((write_process1 = write(p_first_to_second[1], buffer_process1, 80)) == -1)
        fatal("write in process 1");

      if((read_process1 = read(p_second_to_first[0], from_second_process1, 160)) == -1)
        fatal("read in process 1");

      while(buffer_process1[++start_compare] != '\0');
      
      if(compare(buffer_process1, from_second_process1, start_compare + 1) == -1)
        printf("Original string and string from process 2 do not match.\n");
      else
        printf("Original string and string from process 2 match.\n");
      
      wait(&status_childOfOne);
      printf("Terminating Process 1\n");

      return(status_childOfOne);
  }

  /* remainder of routine executed by child of Process 1, aka. Process 2 */

  int p_second_to_third[2], status_childOfTwo;
  int p_third_to_second[2]; 
  
  ssize_t read_process3, write_process3;
  char text_process3[80], new_text_process3[80];
  int final_position = -1, to_upper = 0;

  ssize_t write_process2;
  char buffer_process2[80],rev_process2[80], from_third_process2[80], 
       *cat_process2;
  ssize_t read_process2;
  int old_position = -1, new_position = 0;

  if(pipe(p_second_to_third) == -1)
    fatal("pipe call in connect");

  if(pipe(p_third_to_second) == -1)
    fatal("pipe call in connect");

  switch(fork())
  {
    case -1:	/* error */
      fatal("2nd fork call in connect");
    case 0:	/* Child, Process 3 */
      close(p_second_to_third[1]);
      close(p_third_to_second[0]);

      if((read_process3 = read(p_second_to_third[0], text_process3, 80)) == -1)
        fatal("read in process 3");
      
      /* The third process should convert the string to uppercase,
       * print the string, and send it back to the second process.
       */
      while(text_process3[++final_position] != '\0');
      for(to_upper = 0; to_upper < final_position; to_upper++) 
      { 
        if((text_process3[to_upper] > 96) && (text_process3[to_upper] < 123))
          new_text_process3[to_upper] = text_process3[to_upper] - 'a' + 'A';  //make uppercase   
        else
          new_text_process3[to_upper] = text_process3[to_upper];
      }            
      new_text_process3[to_upper] = '\0';
      printf("%s\n", new_text_process3);

      if((write_process3 = write(p_third_to_second[1], new_text_process3, 80)) == -1)
        fatal("write in process 3");

      printf("Termininating Process 3\n");

      break;
    default:

      close(p_first_to_second[1]);
      close(p_second_to_third[0]);
      close(p_third_to_second[1]);
      close(p_second_to_first[0]);

      if((read_process2 = read(p_first_to_second[0], buffer_process2, 80)) == -1)
        fatal("read in process2");

      /* The second process should reverse the string
       * ,print the reversed string, and send it to the
       * third process     
       */ 
      while(buffer_process2[++old_position] != '\0');
      while(old_position >= 0)
        rev_process2[new_position++] = buffer_process2[--old_position];
      rev_process2[new_position] = '\0';
      printf("%s\n", rev_process2);

      if((write_process2 = write(p_second_to_third[1], rev_process2, 80)) == -1)
        fatal("write in process 2");

      wait(&status_childOfTwo);
         
      if((read_process2 = read(p_third_to_second[0], from_third_process2, 80)) == -1)
        fatal("read in process2");

        /* concatenate the strings */
      cat_process2 = malloc(snprintf(NULL, 0, "%s %s",
                            from_third_process2, buffer_process2) + 1);
      sprintf(cat_process2, "%s %s", from_third_process2, buffer_process2);                
      printf("%s\n", cat_process2);

      if((write_process2 = write(p_second_to_first[1], cat_process2, 160)) == -1)
        fatal("write in process2");

      printf("Termininating Process 2\n");

      return(status_childOfTwo);
  }
  return(0); 
} //connect()
/*------------------------------------------*/
/*	Problem 3	*/
int Fork_variable_test()
{
  int test_integer = 1;
  char test_phrase[] = "1 test phrase";
  char modified_phrase[13];

  int status;

  int p[2];
  if(pipe(p) == -1)
    fatal("pipe call in Fork-Variable-test");

  switch(fork())
  {
    case -1:
      fatal("1st fork call in Fork-variable-test\n");
    case 0:
      printf("test_integer in child: %d. test_phrase in child: %s\n",
             test_integer, test_phrase);
      test_phrase[0] = '2';        
      printf("Modified test_phrase in child: %s\n", test_phrase);

      close(p[0]);

      if(write(p[1], test_phrase, 13) == -1)
        fatal("error in write");
      break;
    default:
        /* demonstrates that the program variables in parent and child 
         * processes have the same values immediately after fork returns
         */    
      printf("test_integer in parent: %d. test_phrase in parent: %s\n",
             test_integer, test_phrase);
      wait(&status);
        /* but are then independent of each other */
      printf("Test phrase after 'wait' system call in parent: %s\n", test_phrase);

      close(p[1]);

      if(read(p[0], modified_phrase, 13) == -1)
        fatal("error in read");
     
      if(compare(test_phrase, modified_phrase, 0) == -1)
        printf("The test phrases are different.\n");
      else 
        printf("The test phrases are the same.\n");

      return(status);
  }           
}

int File_descriptor_test()
{
  /* demonstrates that file descriptors in a parent and child
   * process have the same meaning immediately after fork returns
   */
  int fd;
  int status;

  if((fd = open("FileDescriptorTest.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1)
    fatal("Open error in File-Descriptor-Test");
  
  switch(fork())
  {
    case -1:
      fatal("Error in form in File-Descriptor-Test");
    case 0:
      if(write(fd, "Hello ,", 7) == -1)
        fatal("Error in write in child.");

      /*  changes to the file descriptors in one process do not
       *  impact the other.
       */
       fd = 1;
       if(write(fd, "Changing file descriptor!\n", 26) == -1) 
         fatal("Error in writing to stdout.");
       break;
     default:
       if(write(fd, "World!\n", 7) == -1)
         fatal("Error in write in parent.");
  
        wait(&status);

       if(write(fd, "No change in file descriptor!\n", 30) == -1)
         fatal("Error in write in parent.");   
     
        return(status);    
  }
  return 0;
} 
/*------------------------------------------*/

/*	Problem 4	*/

#define MAXITR 5
unsigned int count1 = 0;
unsigned int count2 = 0;

static void catch1(int sig, siginfo_t *siginfo, void *context)
{
  printf("Count is %ld, my pid is %ld\n", count1, (long)getpid());
  count1 = 0;
}

static void catch2(int sig, siginfo_t *siginfo, void *context)
{
  printf("Count is %ld, my pid is %ld\n", count2, (long)getpid());
  count2 = 0;
}

void AlrmTest()
{
  pid_t pid1, pid2, ppid;
  void catch1(int, siginfo_t *, void *),
       catch2(int, siginfo_t *, void *);
  static struct sigaction act, oact;

  if((pid1 = fork()) == -1) 
  {
    fatal("Error in fork in AlrmTest");
  }
  else if(pid1 == 0)	/* child 1 */
  {
    sigfillset(&(act.sa_mask));
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = catch1;
    sigaction(SIGALRM, &act, &oact);

    ppid = getppid();

    for(;;)
    {
      count1++;
    }

  }
  else
  {
    if((pid2 = fork()) == -1)
      fatal("Error in second fork in AlrmTest");
    else if(pid2 == 0)	/* child 2 */
    {
      sigfillset(&(act.sa_mask));
      act.sa_flags = SA_SIGINFO;
      act.sa_sigaction = catch2;
      sigaction(SIGALRM, &act, &oact);

      ppid = getppid();

      for(;;)
      {
        count2++;
      }

    }
    else
    {
      int send;

      for(send = 0; send < MAXITR; send++)
      {
        sleep(1);
        kill(pid1, SIGALRM);
        kill(pid2, SIGALRM);
      }
    }
  }
}
    
  
/*------------------------------------------*/
/*	Problem 5	*/

char path[80];

int listAll( int depth, char * path )
{
  DIR *dp=NULL;
  struct dirent *d;
  struct stat buf;

  if((dp = opendir(path)) == NULL) {
    return(-2);
  } 
  
  while((d = readdir(dp)) != NULL) {

    if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
      continue;

    int path_name_length = strlen(path) + strlen(d->d_name) + strlen("/");
    char newpath[path_name_length + 1];
    /*------------------- Create new path----------------------*/
    int copy, old;

    for(copy = 0; copy < strlen(path);copy++)
      newpath[copy] = path[copy];

      newpath[copy++] = '/';
  
    for(old = 0; copy < path_name_length, old < strlen(d->d_name); copy++, old++)
      newpath[copy] = d->d_name[old];
    
    newpath[copy] = '\0';

    int indent = 0;
    for(indent = 0; indent < depth; indent++)
      printf("  ");

    /*--------------------------------------------------------*/

    printf("%s\n", d->d_name);
     
    if(stat(newpath, &buf) == -1)
    {
      printf("Couldn't stat %s\n", newpath);
      return(-1);
    }
      //Depth-first-search
    if(S_ISDIR(buf.st_mode))
      listAll(depth + 1, newpath);
  }
}

void directory_traverse_depth_first()
{
  scanf("%s", path);
  listAll(0, path);
}
/*------------------------------------------*/
/* ---------------Problem 6 ----------------*/

void c2_action(int sig) 
{
} //c1act()

void c1_action(int sig) 
{
} //c1act()

void p_action(int sig) 
{
} //c1act()

int process_tree(int n, int node)
{
  if(n == 0) 
    return;

  pid_t left_child, right_child, ppid;  
  void p_action(int), c1_action(int), c2_action(int);
  static struct sigaction pact, c1act, c2act;

  /* set SIGUSR1 action for parent */
  pact.sa_handler = p_action;
  sigaction(SIGUSR1, &pact, NULL);
 
  left_child = fork();

  if(left_child == -1) {
    perror("process_tree");
    return(-1);
  }
  else if(left_child == 0)
  { 
    c1act.sa_handler = c1_action;
    sigaction(SIGUSR1, &c1act, NULL);
   
    ppid = getppid();
      
    kill(ppid, SIGUSR1);
    pause();
    process_tree(n - 1, node + 1);
  }
  else
  {
    pause(); 
    printf("I am process %d, my process identifier is %ld.\n", node, getpid());
    
    kill(left_child, SIGUSR1);    

    sleep(1); 

    right_child = fork();

    if(right_child == -1) {
      perror("process_tree");
      return(-1);
    }
    else if(left_child == 0)
    {
      c2act.sa_handler = c2_action;
      sigaction(SIGKILL, &c2act, NULL);
      process_tree(n - 1, node + 2); 
    }    
    else
      wait(NULL);

  } // else
}

/*------------------------------------------*/
main(int argc, char* argv[])
{
  //Odds_then_evens(argv[1]); 
  //connect();
  //Fork_variable_test();
  //File_descriptor_test();
  //AlrmTest();
  //directory_traverse_depth_first();
  /*  
  char * input = argv[1]; 
  int depth = input[0] - '0';
  int calls = 1;
  int i;
  for(i = 1; i <= depth; i++)
  {
    calls = 2*calls;
  }
  process_tree(calls - 1,1);
  */
  return(0);
}








