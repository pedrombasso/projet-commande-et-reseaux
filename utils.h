#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

// to avoid warnings
#include <unistd.h>
#include <arpa/inet.h>

// our includes
#include <time.h>

#define ERROR -1

struct mesg
{
	int id;
	float position[6];
	float control[1];
};

int timeDiffMs( struct timespec end, struct timespec begin )
{
	int diffSec = ( end.tv_sec - begin.tv_sec ) * 1000;
	int diffNano = ( end.tv_nsec - begin.tv_nsec ) / 1000000;
	
	return diffSec + diffNano; 
}