#include "utils.h"

const char* STD_ADDR = "127.0.0.1";

int main (int argc, char *arg[])
{
	int result;
	int nsend;
	int nconnect;
	struct mesg message;
	int addr;
	long int Te;

	struct sockaddr_in sockAddr, sock;
	int serveur, client, err, nConnect, longaddr , results, resultr;
	


	serveur=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
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
	
	addr=sizeof(sockAddr);

	message.label=0.0;
	message.position[0]=0.0;
	message.control[0]=0;
	Te=200000; // Te=100ms

	fcntl(serveur,F_SETFL,fcntl(serveur,F_GETFL) | O_NONBLOCK); 

	do
	{
		usleep(Te);

		message.label++;
		results=sendto(serveur,&message,sizeof(message),0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
		//resultr=recvfrom(serveur,&message,sizeof(message), 0,(struct sockaddr*)&sockAddr,&addr);

		printf("\n client : \n  label=%lf position=%lf control=%lf rr=%d rs=%d ",message.label,message.position, message.control, resultr, results );

	} while(message.label<100.0);

	close(serveur);

	return 0;

}



