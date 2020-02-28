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

	fcntl(communicatorServer,F_SETFL,fcntl(communicatorServer,F_GETFL) | O_NONBLOCK); 

	
	
	message.label=0.0;
	memset( message.position, 0, sizeof(float)*6);
	message.control[0]=0;

	int userInput, exitController;
	exitController = 0;
	do
	{
		printf("Select robot position:\n\t0 - Initial Position\n\t1 - Position 1\n");
		scanf("%d", &userInput);

		switch( userInput )
		{
			case 0:
				memset( message.position, 0, sizeof(float)*6);
				break;

			case 1:
				message.position[0] = -0.5;
				message.position[1] = 0.5;
				message.position[2] = 0.25;
				message.position[3] = 0.1;
				message.position[4] = 0.3;
				message.position[5] = 1;
				break;
			
			default:
				printf("BYE BYE\n");
				exitController = 1;
				break;

		}
		
		// Te=200000; // Te=100ms
		// usleep(Te);

		message.label++;
		results=sendto(communicatorServer,&message,sizeof(message),0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
		//resultr=recvfrom(communicatorServer,&message,sizeof(message), 0,(struct sockaddr*)&sockAddr,&addr);

		printf("\n client : \n  label=%f position=%f control=%f rr=%d rs=%d ",message.label,message.position[0], message.control[0], resultr, results );

	} while( exitController != 1 );
	


	close(communicatorServer);

	return 0;

}



