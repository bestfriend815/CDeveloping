
/****************************
 * Author: Harsimran Kumar; Ahmad Mando
 * des_inputs.c
 * Assignment-1
 * This program 'des_inputs' prompts the user for inputs to the controller.
 * This program is simulating all of the input events from the devices;
 * e.g., card readers, door latches, scale.
 ****************************/

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <errno.h>

#include "des-mva.h"

//do the error check for the controller and input: usage check

int main(int argc, char* argv[])
{

	    //checking the number of command line arguments
		if (argc != 2) {
			perror("Invalid number of arguments.");
			exit(EXIT_FAILURE);
		}

	    pid_t Pid_controller = atoi(argv[1]);

		Person person;


		// establish a connection --- Phase I
		// connecting to the des_controller which is a server
      	int coid = ConnectAttach (ND_LOCAL_NODE, Pid_controller, 1, _NTO_SIDE_CHANNEL, 0);

	    // in case the connection is not successful
		if (coid == -1)
		{
			printf("Cannot Connect to Server. Please try again\n");
			return EXIT_FAILURE;
		}

		while(1)
		{
			 // Display all the options - INPUT EVENT
			 printf("\n Please Enter the event type (ls= left scan, rs= right scan, ws= weight scale, lo =left open, ro=right open, lc = left closed, rc = right closed , gru = guard right unlock, grl = guardright lock, gll=guard left lock, glu = guard left unlock, exit = exit programs)\n");
			 scanf("%s", &person.event);

			 // If the event is ls or rs, prompt for the person_id.
			 if(strcmp(person.event, "ls") == 0 || strcmp(person.event, "rs") == 0)
			 {
			 	    printf("%s", outMessage[0]);

			 		scanf("%d", &person.id);
			 		printf("\nPerson scanned ID, ID is = %d", person.id);//display
			 }

			 // If the event is ws, prompt for the weight.
			 //use arrays in header file
			 else if(strcmp(person.event, "ws") == 0)
			 {
				    printf("%s",outMessage[5]);
			 		scanf(" %d", &person.weight);

			 		printf("\nPerson weighed, Weight = %d", person.weight);//display
			 }

			 // send the message --- Phase II
			 // Once the input is finished, send a message to the des_controller program
			 // to provide the input “event”, and loop back to prompt again.

			 printf("test msg send");
			 if (MsgSend (coid, &person, sizeof (person), NULL ,0) == -1)
			 {
			 	fprintf (stderr, "Error during MsgSend\n");
			 	perror (NULL);
			 	exit (EXIT_FAILURE);
			 }




		}

	//Disconnect from the channel --- Phase III
	ConnectDetach(coid);
	return EXIT_SUCCESS;
}
