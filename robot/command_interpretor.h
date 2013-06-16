/*
 * command_interpretor.h
 *
 *  Created on: Nov 22, 2012
 *      Author: alexs
 */

#ifndef COMMAND_INTERPRETOR_H_
#define COMMAND_INTERPRETOR_H_


typedef struct
{
	int left;
	int right;
}cmd_motor_lr_t;


typedef struct
{
	 int cmd_parameter_nr;
	 int (*cmd_action)(char *cmd_parameter);
	 const char *cmd_str;
}command_struct_t;

#endif /* COMMAND_INTERPRETOR_H_ */
