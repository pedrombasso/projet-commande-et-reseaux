#include "../utils.h"

int main (int argc, char *arg[])
{

	struct mesg message;
	int result, nsend;
	struct sockaddr_in sockServer, sockClient;
	int controllerClient, commandeServer, err, nConnect;
	unsigned int longaddr;
	int n , i;
	double  Un,  Sn,  Snp , K;
	double tau, dt, A, B;
	int Rt;  
	
	
	// ###### Server part

	controllerClient=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	sockServer.sin_family=PF_INET;
	sockServer.sin_port=htons(2000); 
	sockServer.sin_addr.s_addr=0;
	longaddr=sizeof(sockServer);

		err=bind(controllerClient,(struct sockaddr*)&sockServer,longaddr);
	 	if(err==ERROR) {
		    printf("\n erreur de bind du controllerClient UDP!! \n");
		}

	fcntl(controllerClient,F_SETFL,fcntl(controllerClient,F_GETFL) | O_NONBLOCK); 



    // ####### Client Part
	
	commandeServer=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	sockClient.sin_family=PF_INET;
	sockClient.sin_port=htons(4545); 
	sockClient.sin_addr.s_addr=inet_addr("127.0.0.1");
	fcntl(commandeServer,F_SETFL,fcntl(commandeServer,F_GETFL) | O_NONBLOCK); 
	
	// ### end client part
	

	long int Te = 200000; // Te=100ms
	// long int Tdelay = 1000000; // 1 seg

	int rcvReturn = ERROR;
	int sendReturn = ERROR;

	printf("######### Starting Communicator Server ##########\n\n\n");

	do
	{

		rcvReturn = recvfrom( controllerClient,&message,sizeof(message), 0,(struct sockaddr*)&sockServer,&longaddr );
		if( rcvReturn != ERROR )
		{
			
			printf("RCVD from CLIENT\n");
			printMessage(message);
			
			// simulated delay, each 10 seconds we have to change it's value, from 20ms to 1s
			int randomDelay = rand() % (MAXTIME + 1 - MINTIME) + MINTIME;
			usleep( randomDelay );

			printf("Sending with delay of %d ms\n", randomDelay/1000 );
			sendReturn=sendto( commandeServer,&message,sizeof(message),0,(struct sockaddr*)&sockClient,sizeof(sockClient));
			// results=sendto(controllerClient,&message,sizeof(message),0,(struct sockaddr*)&sockServer,sizeof(sockAddr));

		}


		// Loopback receive from the comande
		rcvReturn=recvfrom( commandeServer,&message,sizeof(message), 0,(struct sockaddr*)&sockClient,&longaddr );
		if( rcvReturn != ERROR )
		{
			printf("RCVD from VREP\n");
			printMessage(message);
			
			// simulated delay, each 10 seconds we have to change it's value, from 20ms to 1s
			int randomDelay = rand() % (MAXTIME + 1 - MINTIME) + MINTIME;
			usleep( randomDelay );
			
			printf("Sending with delay of %d ms\n", randomDelay/1000 );
			sendReturn=sendto( controllerClient,&message,sizeof(message),0,(struct sockaddr*)&sockServer,sizeof(sockServer));

			printf("################################################\n\n\n");
		}


	} while(1);


	close(controllerClient);

	return 0;

}

