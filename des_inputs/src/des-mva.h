/*
 * des-mva.h
 *
 *  An example H file that you can use as a model for defining enums for the inputs, outputs, and state names.
 *  This approach will make your code more readable :)
 */

#ifndef DES_MVA_H_
#define DES_MVA_H_

#define NUM_STATES 14
typedef enum
{
	    START_STATE = 0,
	    READY_STATE = 1,
	    LEFT_SCAN_STATE = 2,
	    RIGHT_SCAN_STATE = 3,
	    GUARD_LEFT_UNLOCK_STATE = 4,
	    GUARD_RIGHT_UNLOCK_STATE = 5,
	    LEFT_OPEN_STATE = 6,
	    RIGHT_OPEN_STATE = 7,
	    WEIGHT_SCAN_STATE = 8,
	    LEFT_CLOSE_STATE = 9,
	    RIGHT_CLOSE_STATE = 10,
	    GUARD_LEFT_LOCK_STATE = 11,
	    GUARD_RIGHT_LOCK_STATE = 12,
	    EXIT_STATE = 13
} State;

#define NUM_INPUTS 12
typedef enum
{
	LEFT_SCAN_INPUT = 0,
	RIGHT_SCAN_INPUT = 1,
	GUARD_LEFT_UNLOCK_INPUT = 2,
	GUARD_RIGHT_UNLOCK_INPUT = 3,
	LEFT_OPEN_INPUT = 4,
	RIGHT_OPEN_INPUT = 5,
	WEIGHT_SCAN_INPUT = 6,
	LEFT_CLOSE_INPUT = 7,
	RIGHT_CLOSE_INPUT = 8,
	GUARD_LEFT_LOCK_INPUT = 9,
	GUARD_RIGHT_LOCK_INPUT = 10,
	EXIT_INPUT = 11
} Input;

#define NUM_OUTPUTS 10
typedef enum
{
		SCAN_MSG = 0,
		GUARD_LEFT_UNLOCK_MSG = 1,
		GUARD_RIGHT_UNLOCK_MSG = 2,
		LEFT_OPEN_MSG = 3,
		RIGHT_OPEN_MSG = 4,
		WEIGHT_SCAN_MSG = 5,
		GUARD_LEFT_LOCK_MSG = 6,
		GUARD_RIGHT_LOCK_MSG = 7,
		EXIT_CONTROLLER = 8,
		EXIT_DISPLAY = 9
} Output;

const char *outMessage[NUM_OUTPUTS] =
{
		 "Enter the Person's ID:\n",
		 "Left door unlocked by Guard\n",
		 "Right door unlocked by Guard\n",
		 "Person opened left door\n",
		 "Person opened right door\n",
		 "Enter the Person's weight:\n",
		 "Left door locked by Guard\n",
		 "Right door locked by Guard\n",
		 "Exiting controller\n",
		 "Exit Display\n"
};

const char *inMessage[NUM_INPUTS] = {
		"ls",
		"rs",
		"glu",
		"gru",
		"lo",
		"ro",
		"ws",
		"lc",
		"rc",
		"gll",
		"grl",
		"exit"};
struct person_details{
	int id;
	int weight;
	int state;
	char event[128];
} typedef Person;

typedef struct {
	int msg;
	Person *p;
} Display;

void *ready_state();
void *left_scan();
void *right_scan();
void *guard_left_unlock();
void *guard_right_unlock();
void *left_open();
void *right_open();
void *weight_scan();
void *left_close();
void *right_close();
void *guard_left_lock();
void *guard_right_lock();
void exit_state();
void invalid_command();
int strcmp();
int getpid();
int sleep();
#endif /* DES_MVA_H_ */
