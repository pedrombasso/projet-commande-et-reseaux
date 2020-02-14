#include "utils.h"

int main (int argc, char *arg[])
{

	struct mesg message;
	int result, nsend;
	struct sockaddr_in sockAddr, sock;
	int serveur, client, err, nConnect, longaddr;
	int n , i, results, resultr ;
	long int  Te;
	double  Un,  Sn,  Snp , K;
	double tau, dt, A, B;
	int Rt;  
	

	serveur=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	sockAddr.sin_family=PF_INET;
	sockAddr.sin_port=htons(2000); 
	sockAddr.sin_addr.s_addr=0;
	longaddr=sizeof(sockAddr);

		err=bind(serveur,(struct sockaddr*)&sockAddr,longaddr);
	 	if(err==ERROR) {
		    printf("\n erreur de bind du serveur UDP!! \n");
		}

	message.label=0.0;
	message.position[0]=0.0;
	message.control[0]=0.0;


	Te=200000; // Te=100ms

	results=ERROR;
	resultr=ERROR;

	fcntl(serveur,F_SETFL,fcntl(serveur,F_GETFL) | O_NONBLOCK); 
	 
	do
	{

		usleep(Te); 

		resultr=recvfrom(serveur,&message,sizeof(message), 0,(struct sockaddr*)&sockAddr,&longaddr);

		printf("\n server : \n  label=%lf position=%lf control=%lf rr=%d rs=%d ",message.label,message.position, message.control, resultr, results );


		// results=sendto(serveur,&message,sizeof(message),0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));


	} while(message.label<100.0);


	close(serveur);

	return 0;

}

