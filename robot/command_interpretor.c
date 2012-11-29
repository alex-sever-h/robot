/*
 * command_interpretor.c
 *
 *  Created on: Nov 22, 2012
 *      Author: alexs
 */


#include "bluetooth.h"
#include "usart.h"
#include "state_machine.h"
#include "command_interpretor.h"
#include "libopencm3/cm3/common.h"

#define MOTOR_LR_QUEUE 0x00000001
const char MOTOR_LR_CMD[] = "MLR";


#define END_CMD        '\r'

cmd_motor_lr_t cmd_motor_lr;

int parameters[5];

u32 command_queue;
int cmd_index;

int motor_lr(void);
int motor_angle(void);

command_struct_t cmds[]=
{
		{
				.cmd_str          = "M_LR",
				.cmd_parameter_nr = 2,
				.cmd_action       = motor_lr,
		},
		{
				.cmd_str          = "M_ANGLE",
				.cmd_parameter_nr = 2,
				.cmd_action       = motor_angle,
		},
		{
				.cmd_str          = "CONNECT",
				.cmd_parameter_nr = 15,
				.cmd_action       = state_connect,
		},
		{
				.cmd_str          = "DISCONNECT",
				.cmd_parameter_nr = 15,
				.cmd_action       = state_disconnect,
		}
};

const int cmds_len = sizeof(cmds)/sizeof(command_struct_t);


int check_command(char cmd_byte, const char *cmd_str, u32 cmd_queue_flag, int param_nr, int cmd_index)
{

	return 0;
}



void interpret_byte(char cmd)
{
	int res;

	if(command_queue == 0)
		cmd_index = 0;

	res = check_command(cmd, MOTOR_LR_CMD, MOTOR_LR_QUEUE, 2, cmd_index);
	if(res)
		motor_control_lr(parameters[0], parameters[1]);


}


int motor_lr(void)
{

}

int motor_angle(void)
{

}

inline int is_endchar(char c)
{
	return c == END_CMD;
}














