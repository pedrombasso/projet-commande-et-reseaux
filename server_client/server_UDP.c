#include "utils.h"

int main (int argc, char *arg[])
{

	struct mesg message;
	int result, nsend;
	struct sockaddr_in sockServer, sockClient;
	int controllerClient, commandeServer, err, nConnect, longaddr;
	int n , i, results, resultr ;
	long int  Te;
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
	
	message.label=0.0;
	message.position[0]=0.0;
	message.control[0]=0.0;

	Te=200000; // Te=100ms

	results=ERROR;
	resultr=ERROR;

	do
	{

		usleep(Te); 

		resultr=recvfrom( controllerClient,&message,sizeof(message), 0,(struct sockaddr*)&sockServer,&longaddr );

		printf("\n Received from Controller Client : \n  label=%lf position=%lf control=%lf rr=%d rs=%d ",message.label,message.position[0], message.control[0], resultr, results );

		// delay simulado

		results=sendto( commandeServer,&message,sizeof(message),0,(struct sockaddr*)&sockClient,sizeof(sockClient));
		// results=sendto(controllerClient,&message,sizeof(message),0,(struct sockaddr*)&sockServer,sizeof(sockAddr));


	} while(message.label<100.0);


	close(controllerClient);

	return 0;

}

