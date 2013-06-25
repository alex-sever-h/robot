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

#define END_CMD        '\r'

cmd_motor_lr_t cmd_motor_lr;

u32 command_queue;
int cmd_index;

int motor_lr(char *cmd_parameter);
int motor_angle(char *cmd_parameter);
int motor_fb_timed(char *cmd_parameter);
int motor_rot_timed(char *cmd_parameter);
int motor_interrupt(char *cmd_parameter);

const char m_lr[]  = "M_LR";
const char m_ft[]   = "M_FT";
const char m_rt[]  = "M_RT";
const char m_ang[] = "M_ANGLE";
const char m_int[] = "M_INTERRUPT";
const char cnct[]    = "CONNECT  ";
const char discnct[] = "DISCONNECT  ";


command_struct_t cmds[]=
{
		{
				.cmd_str          = m_lr,
				.cmd_parameter_nr = 2,
				.cmd_action       = motor_lr,
		},
		{
				.cmd_str          = m_ft,
				.cmd_parameter_nr = 1,
				.cmd_action       = motor_fb_timed,
		},
		{
				.cmd_str          = m_rt,
				.cmd_parameter_nr = 1,
				.cmd_action       = motor_rot_timed,
		},
		{
				.cmd_str          = m_ang,
				.cmd_parameter_nr = 2,
				.cmd_action       = motor_angle,
		},
		{
				.cmd_str          = m_int,
				.cmd_parameter_nr = 0,
				.cmd_action       = motor_interrupt,
		},
		{
				.cmd_str          = cnct,
				.cmd_parameter_nr = 15,
				.cmd_action       = state_connect,
		},
		{
				.cmd_str          = discnct,
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
	if(strncmp(cmd_string, cmd->cmd_str, strlen(cmd->cmd_str)) == 0)
	{
		int i;

		i = strlen(cmd->cmd_str);

		cmd->cmd_action(&cmd_string[i]);

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


int motor_lr(char *cmd_parameter)
{
	int left_pwn, right_pwm;
	//  if(strlen(cmd_parameter) < 8)
	//    return 0;

	left_pwn  =
			(cmd_parameter[1] - '0') * 100 +
			(cmd_parameter[2] - '0') * 10 +
			(cmd_parameter[3] - '0');
	if (cmd_parameter[0] == '-')
		left_pwn = -left_pwn;

	right_pwm  =
			(cmd_parameter[5] - '0') * 100 +
			(cmd_parameter[6] - '0') * 10 +
			(cmd_parameter[7] - '0');
	if (cmd_parameter[4] == '-')
		right_pwm = -right_pwm;

	motor_control_pwm(left_pwn, right_pwm);

	return 1;
}

int motor_fb_timed(char *cmd_parameter)
{
	int time_ms;

	time_ms =
			(cmd_parameter[1] - '0') * 1000 +
			(cmd_parameter[2] - '0') * 100 +
			(cmd_parameter[3] - '0') * 10 +
			(cmd_parameter[4] - '0');
	if (cmd_parameter[0] == '-')
		time_ms = -time_ms;

	motor_run_time(time_ms);

	return 0;
}

int motor_rot_timed(char *cmd_parameter)
{
	int time_ms;

	time_ms =
			(cmd_parameter[1] - '0') * 1000 +
			(cmd_parameter[2] - '0') * 100 +
			(cmd_parameter[3] - '0') * 10 +
			(cmd_parameter[4] - '0');
	if (cmd_parameter[0] == '-')
		time_ms = -time_ms;

	motor_rot_time(time_ms);
	return 0;
}

int motor_interrupt(char *cmd_parameter)
{
	motor_interrupt_movement();
}

int motor_angle(char *cmd_parameter)
{

	return 0;
}

inline int is_endchar(char c)
{
	return c == END_CMD;
}














