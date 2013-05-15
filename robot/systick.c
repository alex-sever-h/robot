/*
 * systick.c
 *
 *  Created on: Dec 9, 2012
 *      Author: alexs
 */
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/cm3/systick.h>
#include "us_sensor.h"

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


void sys_tick_handler(void)
{
	temp32++;

	/* We call this handler every 1ms so 1000ms = 1s on/off. */
	char buffer[30];

//	if (!(temp32 % 1600))
//	{
//		tty_puts("F_L : ");
//		int_to_a(buffer, up_fl);
//		tty_puts(buffer);
//		tty_puts("++ \r\n");
//
//		tty_puts("F_R : ");
//		int_to_a(buffer, up_fr);
//		tty_puts(buffer);
//		tty_puts("++ \r\n");
//
//		tty_puts("R_L : ");
//		int_to_a(buffer, up_rl);
//		tty_puts(buffer);
//		tty_puts("++ \r\n");
//
//		tty_puts("R_R : ");
//		int_to_a(buffer, up_rr);
//		tty_puts(buffer);
//		tty_puts("++ \r\n");
//	}

	if (!(temp32 % 250))
	{
		tty_puts("F_R : ");
		int_to_a(buffer, f_right_distance);
		tty_puts(buffer);
		tty_puts("mm \r\n");

		tty_puts("F_L : ");
		int_to_a(buffer, f_left_distance);
		tty_puts(buffer);
		tty_puts("mm \r\n");

		tty_puts("R_R : ");
		int_to_a(buffer, r_right_distance);
		tty_puts(buffer);
		tty_puts("mm \r\n");

		tty_puts("R_L : ");
		int_to_a(buffer, r_left_distance);
		tty_puts(buffer);
		tty_puts("mm \r\n");
	}

	if (!(temp32 % 300))
	{
		us_sensor_trigger(RIGHT);
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
