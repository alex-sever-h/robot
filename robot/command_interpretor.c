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

void interpret_byte(char cmd)
{
	static int cmd_index;
	static u32 command_queue;

	// if expecting a motor command
	if (command_queue & MOTOR_LR_QUEUE)
	{

		if (cmd_index < strlen(MOTOR_LR_CMD))     // if still processing the command
		{
			if (cmd != MOTOR_LR_CMD[cmd_index])
				command_queue &= ~MOTOR_LR_QUEUE;
		}

		if (cmd_index >= strlen(MOTOR_LR_CMD) && cmd_index < strlen(MOTOR_LR_CMD) + 2)    // if reading parameters
		{
			parameters[cmd_index - strlen(MOTOR_LR_CMD)] = cmd;
		}
		else      // expect END_CMD
		{
			if(cmd == END_CMD) // accept command
			{
				motor_control_lr(parameters[0], parameters[1]);
				usart_puts(MOTOR_LR_CMD);
				command_queue &= ~MOTOR_LR_QUEUE;
			}
		}

	}

	if (!(command_queue & (MOTOR_LR_QUEUE)))
	{
		command_queue = ~0;
		cmd_index = 0;
	}
	else
	{
		cmd_index++;
	}
}
