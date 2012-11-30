/*
 * command_interpretor.c
 *
 *  Created on: Nov 22, 2012
 *      Author: alexs
 */

#include <string.h>
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

int mySTRCMP(const char *s1, const char *s2)
{
	int i, len1, len2;
	len1 = strlen(s1);
	len2 = strlen(s2);

	for(i = 0; i < len1 && i < len2; i++)
	{
		if(s1[i] != s2[i])
			return 1;
	}

	return 0;
}

int check_command(command_struct_t *cmd, char * cmd_string)
{
	if(mySTRCMP(cmd_string, cmd->cmd_str) == 0)
	{
		int i;

		for(i = 0; i < cmd->cmd_parameter_nr; ++i)
		{
			parameters[i] = cmd_string[strlen(cmd->cmd_str) + i];
		}

		cmd->cmd_action();

		return 1;
	}
	else
	{
		return 0;
	}
}


int interpret_command(char *cmd_buffer)
{
	int i;

	for(i = 0; i < cmds_len; ++i)
	{
		if(check_command(&cmds[i], cmd_buffer))
			return 1;
	}
	return 0;
}


void interpret_byte(char cmd)
{



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














