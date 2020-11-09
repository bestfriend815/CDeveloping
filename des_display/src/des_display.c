
/****************************
 * Author: Harsimran Kumar; Ahmad Mando
 * des_display.c
 * Assignment-1
 * This program des_display displays the status of the system – which door is open / closed,
 * if there is a user waiting to enter from the left or right, etc.
 * The program should run in the background and print out status information to the console
 * each time a status update is sent to it using a message from the des_controller program.
 * The des_display program can print out its process id when it first starts up
 ****************************/

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <errno.h>

#include "../../des_inputs\src\des-mva.h"

int main(int argc, char* argv[])
{
Display d;

int display_chid;
printf("The Display process ID is :%d\n",getpid());
// Phase 1 creating the channel
display_chid= ChannelCreate(0);
//phase 2
while(1){
    int rcvid=MsgReceive(display_chid,&d,sizeof(d),NULL);
    if(rcvid==-1){
        printf("Error receiving message from Controller\n");
        return EXIT_FAILURE;
    }

    switch (d.msg)
    {
                case LEFT_SCAN_STATE:

                    break;
                case RIGHT_SCAN_STATE:
                    break;
                case WEIGHT_SCAN_STATE:
                    break;
                case LEFT_CLOSE_STATE:
                    break;
                case RIGHT_CLOSE_STATE:
                    break;
                case GUARD_LEFT_UNLOCK_STATE:
                    printf(outMessage[GUARD_LEFT_UNLOCK_MSG]);
                    break;
                case GUARD_RIGHT_UNLOCK_STATE:
                    printf(outMessage[GUARD_RIGHT_UNLOCK_MSG]);
                    break;
                case LEFT_OPEN_STATE:
                    printf(outMessage[LEFT_OPEN_MSG]);
                    break;
                case RIGHT_OPEN_STATE:
                    printf(outMessage[RIGHT_OPEN_MSG]);
                    break;
                case GUARD_LEFT_LOCK_STATE:
                    printf(outMessage[GUARD_LEFT_LOCK_MSG]);
                    break;
                case GUARD_RIGHT_LOCK_STATE:
                    printf(outMessage[GUARD_RIGHT_LOCK_MSG]);
                    break;
                case EXIT_STATE:
                    printf(outMessage[EXIT_CONTROLLER]);
                    printf(outMessage[EXIT_DISPLAY]);
                    return EXIT_SUCCESS;
                    break;
                default:
                    printf("Invalid Command\n");
                    break;
                }
                continue;
    int reply_id=MsgReply(rcvid,EOK,NULL,0);
    if(reply_id==-1){
        printf("Error replying to Controller \n");
    }
}
//phase 3
ChannelDestroy(display_chid);
return EXIT_SUCCESS;
}
