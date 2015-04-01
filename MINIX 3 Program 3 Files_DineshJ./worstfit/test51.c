#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int i = 0;
  int dummy_count = 0;
  for(i = 0; i < 10000; i++) {
	dummy_count *= 2;
	sleep(1);
	dummy_count /= 2;
  }
}

