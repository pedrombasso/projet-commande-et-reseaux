#include "../utils.h"

const char* STD_ADDR = "127.0.0.1";

int main (int argc, char *arg[])
{
	int result;
	int nsend;
	int nconnect;
	struct mesg message;
	long int Te;

	struct sockaddr_in sockAddr, sock;
	int communicatorServer, client, err, nConnect, longaddr , results, resultr;
	

	communicatorServer=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	sockAddr.sin_family=PF_INET;
	sockAddr.sin_port=htons(2000); 

	if( argc < 2 )
	{
		printf("Working with standard address %s: ", STD_ADDR);
		sockAddr.sin_addr.s_addr=inet_addr(STD_ADDR);
	}
	else
	{
		sockAddr.sin_addr.s_addr=inet_addr(arg[1]);
	}


	message.label=0.0;
	message.position[0]=0.0;
	message.control[0]=0;
	Te=200000; // Te=100ms

	fcntl(communicatorServer,F_SETFL,fcntl(communicatorServer,F_GETFL) | O_NONBLOCK); 

	do
	{
		usleep(Te);

		message.label++;
		results=sendto(communicatorServer,&message,sizeof(message),0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
		//resultr=recvfrom(communicatorServer,&message,sizeof(message), 0,(struct sockaddr*)&sockAddr,&addr);

		printf("\n client : \n  label=%lf position=%lf control=%lf rr=%d rs=%d ",message.label,message.position[0], message.control[0], resultr, results );

	} while(message.label<100.0);

	close(communicatorServer);

	return 0;

}



