/*
 * command_interpretor.c
 *
 *  Created on: Nov 22, 2012
 *      Author: alexs
 */


#include "bluetooth.h"
#include "usart.h"
#include "command_interpretor.h"
#include "libopencm3/cm3/common.h"

#define MOTOR_LR_QUEUE 0x00000001
const char MOTOR_LR_CMD[] = "MLR";

#define END_CMD        '\r'

cmd_motor_lr_t cmd_motor_lr;

int parameters[5];

u32 command_queue;
int cmd_index;



int check_command(char cmd_byte, const char *cmd_str, u32 cmd_queue_flag, int param_nr, int cmd_index)
{
	// if other command --> quit
	if((command_queue != 0) && (command_queue & cmd_queue_flag == 0))
		return 0;

	if (cmd_index >= 0 && cmd_index < (int)strlen(cmd_str))
	{
		if(cmd_str[cmd_index] == cmd_byte)
			command_queue |= cmd_queue_flag;
		else
			command_queue &= ~cmd_queue_flag;
	}
	else if (cmd_index >= strlen(cmd_str) && cmd_index < strlen(cmd_str) + param_nr)
	{
		parameters[cmd_index - strlen(cmd_str)] = cmd_byte;
	}
	else if (cmd_index == strlen(cmd_str) + param_nr)
	{
		if (cmd_str[cmd_index] == '\r')
		{
			command_queue = 0;
			return 1;
		}
		else
		{
			command_queue &= ~cmd_queue_flag;
			return 0;
		}
	}
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
















