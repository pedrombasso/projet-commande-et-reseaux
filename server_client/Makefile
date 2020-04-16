all: serverCommunicator clientController

serverCommunicator: communicator.c
	gcc	-o	serverCommunicator	communicator.c
clientController: controller.c
	gcc	-o	clientController	controller.c

clean: 
	rm serverCommunicator clientController
