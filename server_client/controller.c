#include "../utils.h"

const char* STD_ADDR = "127.0.0.1";

double CalculateKc(int delayMs)
{
    // pour 20ns, Kc = 1
	// pour 1000ns, Kc = 0.5
	return 1.0/(1+(double)delayMs/1000.0); 
	
	// avec cftool sur matlab, on a trouve
	//return 0.01565 * pow( delayMs, -1.001 ) - 0.005962;
}


int main (int argc, char *arg[])
{
	int result;
	int nsend;
	int nconnect;
	struct mesg message, rcvdMessage, desiredPos;
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
	memset( message.control, 0, sizeof(float)*6);
	message.control[0]=0;

	rcvdMessage.id=0;
	memset( rcvdMessage.control, 0, sizeof(float)*6);
	rcvdMessage.control[0]=0;

	desiredPos.id=0;
	memset( desiredPos.control, 0, sizeof(float)*6);
	desiredPos.control[0]=0;

	int userInput, exitController;
	exitController = 0;
	int communicationDelay = 200;
	int rcvReturn = ERROR;
	int sendReturn = ERROR;
	int sinStep = PI*0.05 ;


	printf("Select robot position:\n\t0 - Initial Position\n\t1 - Position 1\n\t2 - Position 2\n\t3 - Position 3\n");
	scanf("%d", &userInput);

	switch( userInput )
	{
		case 0:
			memset( desiredPos.control, 0, sizeof(float)*6);
			break;

		case 1:
			desiredPos.control[0] = -0.9*PI;
			desiredPos.control[1] = 0.4*PI;
			desiredPos.control[2] = -0.25*PI;
			desiredPos.control[3] = 0.0*PI;
			desiredPos.control[4] = 0.0*PI;
			desiredPos.control[5] = 0.0*PI;
			break;

		case 2:
			desiredPos.control[0] = 1.0*PI;
			desiredPos.control[1] = -0.25*PI;
			desiredPos.control[2] = -0.5*PI;
			desiredPos.control[3] = -0.0*PI;
			desiredPos.control[4] = 1.0*PI;
			desiredPos.control[5] = -1.5*PI;

			break;

		case 3:
			desiredPos.control[0] = 0*PI;
			desiredPos.control[1] = 0.25*PI;
			desiredPos.control[2] = 0.25*PI;
			desiredPos.control[3] = 0*PI;
			desiredPos.control[4] = -0.25*PI;
			desiredPos.control[5] = PI;
			break;

		default:
			exitController = 1;
			break;

	}

	while( 1 )
	{
		// calculate kc and speed control
		double Kc = CalculateKc(communicationDelay);

		printf("Kc est: %f", Kc);
		
		message.control[0]=(desiredPos.control[0]-rcvdMessage.control[0])*Kc;
		message.control[1]=(desiredPos.control[1]-rcvdMessage.control[1])*Kc;
		message.control[2]=(desiredPos.control[2]-rcvdMessage.control[2])*Kc;
		message.control[3]=(desiredPos.control[3]-rcvdMessage.control[3])*Kc;
		message.control[4]=(desiredPos.control[4]-rcvdMessage.control[4])*Kc;
		message.control[5]=(desiredPos.control[5]-rcvdMessage.control[5])*Kc;


		printf("\nSending control %d to Server\n", userInput );
		printMessage( message );
		printf("Waiting for response...\n\n\n");

		struct timespec timeStart, timeEnd;
		clock_gettime( CLOCK_REALTIME, &timeStart );
		

		sendReturn=sendto(communicatorServer,&message,sizeof(message),0,(struct sockaddr*)&sockClient,sizeof(sockClient));

		// blocking recevie from Comunicator
		do
		{
			
			rcvReturn=recvfrom( communicatorServer,&rcvdMessage,sizeof(rcvdMessage), 0,(struct sockaddr*)&sockClient,&longaddr );			
			clock_gettime( CLOCK_REALTIME, &timeEnd );	
			communicationDelay = timeDiffMs( timeEnd, timeStart );	


		} while ( rcvReturn == ERROR );
		
		if( communicationDelay >= TIMEOUT )
		{
			printf("Connection Timeout\n\n\n");
		}
		else
		{
			printf("Successfully received message\n");
			printMessage(rcvdMessage);			
			printf("\nTotal time: %d ms\n\n", communicationDelay);
		}

		message.id++;

		printf("#####################################################\n\n");
	}
	

	printf("Closing connection\n");
	close(communicatorServer);

	return 0;

}



