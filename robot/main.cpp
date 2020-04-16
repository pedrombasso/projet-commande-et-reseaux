//
//   C++ - VREP API for manipulator Robotis H
//
//
//
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "../utils.h"

using namespace std;

extern "C" 
{
    #include "extApi.h"
}

int handles[6],all_ok=1;
simxInt handle, error;


void GetHandles(int clientID)
{
	simxChar objectName[100];
	char str[10];
    for (int i=0; i < 6; i++) 
    {
        strcpy(objectName, "joint");
        sprintf(str, "%d", i+1);
        strcat(objectName,str);
        error=simxGetObjectHandle(clientID, objectName, &handle, simx_opmode_oneshot_wait);
        if (error == simx_return_ok)
            handles[i]=handle;
        else 
        {
            printf("Error in Object Handle - joint number %d\n", i);
            all_ok=0;
        }
    }
}
/////////////////////////////////////////////////////////
// Set the join position
//
// Inputs:
//  clientID
//  q : array of the joint values
// Return: 0 if an error occurs in object handling, 1 otherwise
/////////////////////////////////////////////////////////
int SetJointPos(int clientID,  float *q)
{
    //simxChar objectName[100];
    //char str[10];
    //simxInt handle, error;
    //int all_ok=1;

    // Get the table of handles
    /*
    for (int i=0; i < 6; i++) {
        strcpy(objectName, "joint");
        sprintf(str, "%d", i+1);
        strcat(objectName,str);
        error=simxGetObjectHandle(clientID, objectName, &handle, simx_opmode_oneshot_wait);
        if (error == simx_return_ok)
            handles[i]=handle;
        else {
            printf("Error in Object Handle - joint number %d\n", i);
            all_ok=0;
        }
    }
    */
    if (all_ok)
    {
        //Pause the communication thread
        //simxPauseCommunication(clientID, 1);
        // Send the joint target positions
        for (int i=0; i < 6; i++)
            simxSetJointTargetPosition(clientID, handles[i], q[i], simx_opmode_oneshot);
        // Resume the communication thread to update all values at the same time
        //simxPauseCommunication(clientID, 0);
        return 1;
    }
    else
        return 0;
}

int GetJointPos(int clientID,  float *q)
{
    simxChar objectName[100];
    char str[10];
    simxInt handle, error;
    int handles[6], all_ok=1;

    // Get the table of handles
    for (int i=0; i < 6; i++) {
        strcpy(objectName, "joint");
        sprintf(str, "%d", i+1);
        strcat(objectName,str);
        error=simxGetObjectHandle(clientID, objectName, &handle, simx_opmode_oneshot_wait);
        if (error == simx_return_ok)
            handles[i]=handle;
        else {
            printf("Error in Object Handle - joint number %d\n", i);
            all_ok=0;
        }
    }
    if (all_ok) {
        //Pause the communication thread
        simxPauseCommunication(clientID, 1);
        // Send the joint target positions
		float qv;
        for (int i=0; i < 6; i++) {
			simxGetJointPosition(clientID, handles[i], &qv, simx_opmode_oneshot);
			q[i] = qv;
		}
        // Resume the communication thread to update all values at the same time
        simxPauseCommunication(clientID, 0);
        return 1;
    }
    else
        return 0;
}

int main(int argc,char* argv[])
{
    int portVREP=5555;            // the port number where to connect
    int timeOutInMs=5000;       // connection time-out in milliseconds (for the first connection)
    int commThreadCycleInMs=5;  // indicate how often data packets are sent back and forth - a default value of 5 is recommended
    int err;


    // Server socket variables
    int portCommunicator=4545;
    struct mesg message;
	int result, nsend;
	struct sockaddr_in sockServer;
	int communicatorClient, nConnect;
    unsigned int longaddr;


   

    // Connection to the VREP_Server
    int clientID=simxStart((simxChar*)"127.0.0.1",portVREP,true,true,timeOutInMs,commThreadCycleInMs);

    // Server creation
	communicatorClient=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	sockServer.sin_family=PF_INET;
	sockServer.sin_port=htons(portCommunicator); 
	sockServer.sin_addr.s_addr=0;
    longaddr=sizeof(sockServer);

    err=bind( communicatorClient,( struct sockaddr* )&sockServer, longaddr );
    if( err==ERROR ) 
    {
        printf( "\n erreur de bind du communicatorClient UDP!! \n" );
    }
		
	fcntl(communicatorClient,F_SETFL,fcntl(communicatorClient,F_GETFL) | O_NONBLOCK); 
    // end server creation
    

//    float q[7];
//    for (int i=0; i < 6; i++) q[i]=0.0;

    float q[6];
    float qr[6];
    GetHandles(clientID);
    
    for (int i=0; i < 6;i++)
    	q[i]=0.0; // fill q with zeros


    if (clientID!=-1)
    {
       int nbloop=100;
       simxSynchronous(clientID,true);       // Enable the synchronous mode (Blocking function call)
       simxStartSimulation(clientID, simx_opmode_oneshot);

       float dt=0.05;
       
       // here we should open a server to receive the data from the Communicator's client side
    //    int resultr=recvfrom( communicatorClient,&message,sizeof(message), 0,(struct sockaddr*)&sockServer,&longaddr );
    //     printf("\n Received from Controller Client : \n  label=%lf position=%lf control=%lf rr=%d",message.label,message.position, message.control, resultr );

       float q0m=0.5;
       float q1m=0.5;
       float q2m=0.6;
       
       float w=2*M_PI/2.5;
       
       int offsetTime=simxGetLastCmdTime(clientID)/1000;

        int rcvReturn = 0;

       while ( 1 )
       {
           
            // receiving the data from communicator client
            rcvReturn=recvfrom(communicatorClient,&message,sizeof(message), 0,(struct sockaddr*)&sockServer,&longaddr);

        //    printf("\n Received from Controller Client : \n  label=%f position=%f control=%f rr=%d",message.label,message.position[5], message.control, rcvReturn );

           //GetJointPos(clientId,qr);
           //q[0] = 0;//q0m*sin(w*t);  ##### 
           //if (t>2) q[0]=0.4;
           //else
           //q[0]=0;
           
           //q[1]=-q1m*sin(2*w*t);
           //q[2]=q2m*sin(4*w*t);

            if( rcvReturn > 0   )
            {
                //q = message.position;
                memcpy( q, message.position, sizeof( q ) );
                SetJointPos( clientID, q);

                //usleep( 100000 );

                GetJointPos( clientID, q);
                memcpy( message.position, q, sizeof( q ) );

                
                int sendReturn = sendto( communicatorClient,&message,sizeof(message),0,(struct sockaddr*)&sockServer,sizeof(sockServer));

            }


            
           //simxSynchronousTrigger(clientID);
           usleep(dt*1000*1000);
        }

        simxStopSimulation(clientID, simx_opmode_oneshot);

        // Close the connection to the server
        simxFinish(clientID);
    }
    else
        printf("Connection to the server not possible\n");

    return(0);
}
