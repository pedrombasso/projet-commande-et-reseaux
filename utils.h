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

#define ERROR (-1)

struct mesg
{
	double label;
	double position[1];
	double control[1];
};