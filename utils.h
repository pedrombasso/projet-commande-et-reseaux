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
#include <math.h>

#define ERROR -1
#define TIMEOUT 2000
#define MINTIME 20000 //20ms
#define MAXTIME 1000000 // 1s
#define PI 3.1412

struct mesg
{
	int id;
	float control[6];
};

int timeDiffMs( struct timespec end, struct timespec begin )
{
	int diffSec = ( end.tv_sec - begin.tv_sec ) * 1000;
	int diffNano = ( end.tv_nsec - begin.tv_nsec ) / 1000000;
	
	return diffSec + diffNano; 
}

void printMessage (struct mesg message){

	printf("## Message\t ID: %d\n\t - Controle[0] = %f\n\t - Controle[1] = %f\n\t - Controle[2] = %f\n\t - Controle[3] = %f\n\t - Controle[4] = %f\n\t - Controle[5] = %f\n",
					message.id, message.control[0],message.control[1],message.control[2],message.control[3],message.control[4],message.control[5]);
}
 