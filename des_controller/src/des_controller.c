/****************************
 * Author: Harsimran Kumar; Ahmad Mando
 * des_controller.c
 * Assignment-1
 * This program des_controller runs in the background, operates the state machine for the controller,
 * and directly maintains the persistent data and status for the controller.
 * It should have separate functions for the state handler for each state.
 * Each state handler should perform the actions required for that state,
 * send a message to the des_display program to update the display (as required),
 * and then check the exit conditions for that state. When an exit condition is met,
 * the state handler should return the function pointer for the next state handler.
 * The des_controller program should print out its process id when it first starts up.

 ****************************/


Person person;
Display d;
int coid;
int direction; // 1 for left, 2 for right
typedef void *(*StateFunc)();
StateFunc statefunc;

int main(int argc, char* argv[])
{
	pid_t displayPid;
	int rcvid;
	int controller_chid;
	statefunc = ready_state;
		//phase 1 creating a connection
		//checking the number of command line arguments
		if (argc != 2)
		{
			perror("Invalid number of arguments.");
			exit(EXIT_FAILURE);
		}

	    displayPid = atoi(argv[1]);
	    controller_chid = ChannelCreate (0);

	    if (controller_chid == -1)
	    {
	    	perror("\nFailed to create the channel. Exiting. ");
	    	exit (EXIT_FAILURE);
	    }

		printf("The controller is running as PID: %d\n", getpid());

	    coid=ConnectAttach(ND_LOCAL_NODE, displayPid, 1, _NTO_SIDE_CHANNEL, 0);

		if (coid == -1)
		{
			printf("\nCannot Connect to Display. Exiting ");
			return EXIT_FAILURE;
		}

		while(1)
		{
            //receiving the details of the person sent by the input
			rcvid = MsgReceive (controller_chid, &person, sizeof(person), NULL);
			if(rcvid == -1)
			{
			   printf("\nError receiving message from Inputs. Exiting. \n");
			   return EXIT_FAILURE;
			}

			if (strcmp(person.event, inMessage[EXIT_INPUT]) == 0)
			{
				exit_state();
			}
   //testing:
   //printf("test");
			statefunc = (StateFunc)(*statefunc)();
				sleep(1);

				MsgReply(rcvid, EOK, NULL, 0);
		} //end of while
	    ChannelDestroy(controller_chid);
	    ConnectDetach(coid);
	    return EXIT_SUCCESS;

}
//state machine function - ready_state
//return type  - returns a function pointer
//arguments -void
//description - sets the person and decides which door is being used
void *ready_state()
{

	direction = 0;
	person.id = 0;
	person.weight = 0;
	//the display structure has a pointer to the person
	d.p=&person;

	if (strcmp(person.event, inMessage[LEFT_SCAN_INPUT]) == 0)
	{
		printf("here ready state scan ");
		//set state
		direction = 1;
		person.state = LEFT_SCAN_STATE;
		d.msg=LEFT_SCAN_STATE;

		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1)
		{
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_scan;
	}

	else if (strcmp(person.event, inMessage[RIGHT_SCAN_INPUT]) == 0)
	{
		direction = 2;
		person.state = RIGHT_SCAN_STATE;
		d.msg=RIGHT_SCAN_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1)
		{
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return right_scan;
	}
	return ready_state;
}
//state machine function - left_scan
//return type  - returns a function pointer
//arguments -void
//description - asks the guard to scan the person coming from the left door
void *left_scan() {
	printf("here left scan ");
	//action required for this state
	if (strcmp(person.event, inMessage[GUARD_LEFT_UNLOCK_INPUT]) == 0)
	{
		person.state = GUARD_LEFT_UNLOCK_STATE;
		d.msg=GUARD_LEFT_UNLOCK_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1)
		{
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_left_unlock;
	}

	return left_scan;
}

//state machine function - right_scan
//return type  - returns a function pointer
//arguments - void
//description - asks the guard to scan the person coming from the right door
void *right_scan()
{
	if (strcmp(person.event, inMessage[GUARD_RIGHT_UNLOCK_INPUT]) == 0)
	{
		person.state = GUARD_RIGHT_UNLOCK_STATE;
		d.msg=GUARD_RIGHT_UNLOCK_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1)
		{
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_right_unlock;
	}
	return right_scan;
}

//state machine function - guard_left_unlock
//return type  - returns a function pointer
//arguments - void
//description - asks the guard to unlock the left door
void *guard_left_unlock() {
	//action required for this state
	if (strcmp(person.event, inMessage[LEFT_OPEN_INPUT]) == 0) {
		person.state = LEFT_OPEN_STATE;
		d.msg=LEFT_OPEN_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_open;
	}

	return guard_left_unlock;
}


//state machine function - guard_right_unlock
//return type  - returns a function pointer
//arguments - void
//description - asks the guard to unlock the right door
void *guard_right_unlock() {
	if (strcmp(person.event, inMessage[RIGHT_OPEN_INPUT]) == 0)
	{
		person.state = RIGHT_OPEN_STATE;
		d.msg=RIGHT_OPEN_STATE;;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return right_open;
	}
	return guard_right_unlock;
}

//state machine function - left_open
//return type  - returns a function pointer
//arguments - void
//description - asks the guard to open the left door
void *left_open() {
	if (person.weight == 0) {
		if (strcmp(person.event, inMessage[WEIGHT_SCAN_INPUT]) == 0) {
			person.state = WEIGHT_SCAN_STATE;
			d.msg=WEIGHT_SCAN_STATE;
			if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
				perror("MsgSend failed");
				exit(EXIT_FAILURE);
			}
			return weight_scan;
		}
		return left_open;
	}

	if (strcmp(person.event, inMessage[LEFT_CLOSE_INPUT]) == 0) {
		person.state = LEFT_CLOSE_STATE;
		d.msg= LEFT_CLOSE_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_close;
	}
	return left_open;
}

//state machine function - right_open
//return type  - returns a function pointer
//arguments - void
//description - asks the guard to open the right door
void *right_open() {
	if (person.weight == 0) {
		if (strcmp(person.event, inMessage[WEIGHT_SCAN_INPUT]) == 0) {
			person.state = WEIGHT_SCAN_STATE;
			d.msg=WEIGHT_SCAN_STATE;
			if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
				perror("MsgSend failed");
				exit(EXIT_FAILURE);
			}
			return weight_scan;
		}
		return right_open;
	}

	if (strcmp(person.event, inMessage[RIGHT_CLOSE_INPUT]) == 0) {
		person.state = RIGHT_CLOSE_STATE;
		d.msg=RIGHT_CLOSE_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return right_close;
	}

	return right_open;
}
//state machine function - weight_scan
//return type  - returns a function pointer
//arguments - void
//description - scans the weight of the person
void *weight_scan() {
	if (strcmp(person.event, inMessage[LEFT_CLOSE_INPUT]) == 0)
	{
		person.state = LEFT_CLOSE_STATE;
		d.msg=LEFT_CLOSE_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_close;
	}
	return weight_scan;
}

//state machine function - left_close
//return type  - returns a function pointer
//arguments - void
//description - closes the left door
void *left_close() {
	if (strcmp(person.event, inMessage[GUARD_LEFT_LOCK_INPUT]) == 0) {
		person.state = GUARD_LEFT_LOCK_STATE;
		d.msg=GUARD_LEFT_LOCK_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		if(direction == 2)
			return ready_state;
		return guard_left_lock;
	}
	return left_close;
}

//state machine function - right_close
//return type  - returns a function pointer
//arguments - void
//description - closes the right door
void *right_close() {
	if (strcmp(person.event, inMessage[GUARD_RIGHT_LOCK_INPUT]) == 0) {
		person.state = GUARD_RIGHT_LOCK_STATE;
		d.msg=GUARD_RIGHT_LOCK_STATE;;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		if(direction == 1)
			return ready_state;
		return guard_right_lock;
	}
	return right_close;
}

//state machine function - exit_state
//return type  - returns a function pointer
//arguments - void
//description - exit the program
void exit_state() {
	person.state = EXIT_STATE;
	d.msg=EXIT_STATE;;
	//action required for this state
	if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
		perror("MsgSend failed");
		exit(EXIT_FAILURE);
	}
	sleep(2);
	exit(EXIT_SUCCESS);
}


//state machine function - guard_left_lock
//return type  - returns a function pointer
//arguments - void
//description - asks the guard to lock the left door
void *guard_left_lock() {
	//if (direction == 1){
	if (strcmp(person.event, inMessage[GUARD_RIGHT_UNLOCK_INPUT]) == 0) {
		person.state = GUARD_RIGHT_UNLOCK_STATE;
		d.msg=GUARD_RIGHT_UNLOCK_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_right_unlock;
	}

	return guard_right_lock;
}


//state machine function - guard_right_lock
//return type  - returns a function pointer
//arguments - void
//description - asks the guard to lock the right door
void *guard_right_lock() {
	//if (direction == 2) {
	if (strcmp(person.event, inMessage[GUARD_LEFT_UNLOCK_INPUT]) == 0)
	{
		person.state = GUARD_LEFT_UNLOCK_STATE;
		d.msg=GUARD_LEFT_UNLOCK_STATE;
		if (MsgSend(coid, &d, sizeof(d), NULL, 0) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_left_unlock;
	}

	return guard_right_lock;

}

