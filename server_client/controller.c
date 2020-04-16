#include "../utils.h"

const char* STD_ADDR = "127.0.0.1";

int main (int argc, char *arg[])
{
	int result;
	int nsend;
	int nconnect;
	struct mesg message;
	long int Te;

	struct sockaddr_in sockClient, sock;
	int communicatorServer, client, err, nConnect;
	unsigned int longaddr;

	communicatorServer=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	sockClient.sin_family=PF_INET;
	sockClient.sin_port=htons(2000); 

	if( argc < 2 )
	{
		printf("Starting with standard IP \n\n");
		sockClient.sin_addr.s_addr=inet_addr(STD_ADDR);
	}
	else
	{
		sockClient.sin_addr.s_addr=inet_addr(arg[1]);
	}

	fcntl(communicatorServer,F_SETFL,fcntl(communicatorServer,F_GETFL) | O_NONBLOCK); 
	printf("######## Successfull Conncetion with address : %s #########\n\n", STD_ADDR);

	
	message.id=0;
	memset( message.position, 0, sizeof(float)*6);
	message.control[0]=0;

	int userInput, exitController;
	exitController = 0;
	int rcvReturn = ERROR;
	int sendReturn = ERROR;

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
				exitController = 1;
				break;

		}

		message.id++;
		printf("Sending position %d to Server...\n", userInput );

		struct timespec timeStart;
		clock_gettime( CLOCK_REALTIME, &timeStart );

		sendReturn=sendto(communicatorServer,&message,sizeof(message),0,(struct sockaddr*)&sockClient,sizeof(sockClient));

		// blocking recevie from Comunicator
		do
		{
			rcvReturn=recvfrom( communicatorServer,&message,sizeof(message), 0,(struct sockaddr*)&sockClient,&longaddr );
		} while ( rcvReturn == ERROR );
		

		printf("\n Received from Server: \n  id=%d position=%f control=%f rr=%d rs=%d\n ",message.id,message.position[0], message.control[0], rcvReturn, sendReturn );
		
		struct timespec timeEnd;
		clock_gettime( CLOCK_REALTIME, &timeEnd );
	
		printf("It took %d ns for this shit \n", timeDiffMs( timeEnd, timeStart ));
	
	} while( exitController != 1 );
	

	printf("Closing connection\n");
	close(communicatorServer);

	return 0;

}



