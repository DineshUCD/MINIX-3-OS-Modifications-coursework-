#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main( int argc, char *argv[] )
{
	printf("-Test52 running!-\n");
	printf("30000 stack size allocated");
	if( argc < 2 )
		return 1;
	sleep( atoi( argv[1] ) );
	printf("-Test50 ended!-\n");
	return 0;
}
