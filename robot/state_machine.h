/*
 * state_machine.h
 *
 *  Created on: Nov 25, 2012
 *      Author: alexs
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_


typedef enum bt_mode_st
{
	CONNECTED,
	DISCONNECTED
} robot_state_t;


int  state_connect(char *cmd_parameter);
int  state_disconnect(char *cmd_parameter);


#endif /* STATE_MACHINE_H_ */
