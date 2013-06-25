/*
 * systick.c
 *
 *  Created on: Dec 9, 2012
 *      Author: alexs
 */
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/timer.h>
#include <libopencm3/cm3/systick.h>
#include "us_sensor.h"
#include "state_machine.h"
#include "motor_ctrl.h"
#include <stdio.h>

u32 temp32;

void systick_init(void)
{

	/* 72MHz / 8 => 9000000 counts per second */
	systick_set_clocksource(STK_CTRL_CLKSOURCE_AHB);

	/* 72000000/72000 = 1000 overflows per second - every 1ms one interrupt */
	systick_set_reload(72000);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();

}


/* We call this handler every 1ms so 1000ms = 1s on/off. */
void sys_tick_handler(void)
{
	temp32++;

	if (!(temp32 % 100))
	{
		//trigger sensor
		us_sensor_trigger();

		//update robot position
		send_position_update();
	}
}



//specify delay in us (max 1000)
// TODO: if larger than tick
void delay_us(unsigned int  nCount)
{
	u32 sys_tk = systick_get_value();
	sys_tk -= (nCount-1) * 72;

	for(;systick_get_value() > sys_tk;);
}
