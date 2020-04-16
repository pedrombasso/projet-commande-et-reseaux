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
#define TIMEOUT 1000
#define MINTIME 20000 //20ms
#define MAXTIME 1000000 // 1s

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

void printMessage (struct mesg message){

	printf("\t ID: %d\n\t - Position[0] = %f\n\t - Position[1] = %f\n\t - Position[2] = %f\n\t - Position[3] = %f\n\t - Position[4] = %f\n\t - Position[5] = %f\n\t Actual Control = %f",
					message.id,message.position[0],message.position[1],message.position[2],message.position[3],message.position[4],message.position[5], message.control[0]);
}
 