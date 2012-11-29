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


int  state_connect(void);
int  state_disconnect(void);


#endif /* STATE_MACHINE_H_ */
