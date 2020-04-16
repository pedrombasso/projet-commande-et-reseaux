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
		printf("Select robot position:\n\t0 - Initial Position\n\t1 - Position 1\n\t2 - Position 2\n\t3 - Position 3\n");
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

			case 2:
				// message.position[0] = -1.2*PI;
				// message.position[1] = -0.5*PI;
				// message.position[2] = 0.45*PI;
				// message.position[3] = -0.30*PI;
				// message.position[4] = 2.0*PI;
				// message.position[5] = -1.5*PI;
				message.position[0] = -0.5*PI;
				message.position[1] = -0.25*PI;
				message.position[2] = -0.5*PI;
				message.position[3] = -0.0*PI;
				message.position[4] = 1.0*PI;
				message.position[5] = -1.5*PI;

				break;

			case 3:
				message.position[0] = PI;
				message.position[1] = 0.25*PI;
				message.position[2] = 0.25*PI;
				message.position[3] = 0*PI;
				message.position[4] = -0.25*PI;
				message.position[5] = PI;
				break;

			default:
				exitController = 1;
				break;

		}

		message.id++;
		printf("\nSending position %d to Server\n", userInput );
		printMessage( message );
		printf("Waiting for response...\n\n\n");

		struct timespec timeStart, timeEnd;
		clock_gettime( CLOCK_REALTIME, &timeStart );
		int timeCommunication = 0;

		sendReturn=sendto(communicatorServer,&message,sizeof(message),0,(struct sockaddr*)&sockClient,sizeof(sockClient));

		// blocking recevie from Comunicator
		do
		{
			
			rcvReturn=recvfrom( communicatorServer,&message,sizeof(message), 0,(struct sockaddr*)&sockClient,&longaddr );			
			clock_gettime( CLOCK_REALTIME, &timeEnd );	
			timeCommunication = timeDiffMs( timeEnd, timeStart );	


		} while ( rcvReturn == ERROR );
		
		if( timeCommunication >= TIMEOUT )
		{
			printf("Connection Timeout\n\n\n");
		}
		else
		{
			printf("Successfully received message\n");
			printMessage(message);			
			printf("\nTotal time: %d ms\n\n", timeCommunication);
		}

		printf("#####################################################\n\n");
		
			
	} while( exitController != 1 );
	

	printf("Closing connection\n");
	close(communicatorServer);

	return 0;

}



