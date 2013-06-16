/*
 * main.c

 *
 *  Created on: Oct 10, 2012
 *      Author: alexs
 */

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/doc-stm32f.h>

#include "usart.h"
#include "bluetooth.h"
#include "us_sensor.h"
#include "motor_ctrl.h"
#include "systick.h"
//#include "pin_config.h"
#include <stdio.h>


void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOC clock. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR,
			RCC_APB2ENR_IOPAEN |
			RCC_APB2ENR_IOPBEN |
			RCC_APB2ENR_IOPCEN |
			RCC_APB2ENR_AFIOEN );

	/* Enable clocks for GPIO port B (for GPIO_USART3_TX) and USART3. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR,
			RCC_APB2ENR_USART1EN |
			RCC_APB2ENR_TIM1EN   );

	rcc_peripheral_enable_clock(&RCC_APB1ENR,
			RCC_APB1ENR_USART3EN |
			RCC_APB1ENR_TIM3EN   |
			RCC_APB1ENR_TIM2EN   );
}


int main(void)
{
	clock_setup();
	motor_init();

	//init peripherals
	usart_init(921600);
	bt_init(460800);
	motor_init();

	us_sensor_config();

	char buffer[50];
//	sprintf(buffer, "TTY_TEST\r\n");

	bt_puts(buffer);

	tty_puts("TTY TEST\r\n");


	gpio_set_mode(GPIOA,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_PUSHPULL,
			GPIO2 | GPIO3);

	gpio_set(GPIOA, GPIO2 | GPIO3);


	if (bt_check_already_connected())
	{
		state_connect("'0000-00-000000'");
	}
	else
		state_disconnect("'0000-00-000000'");

	systick_init();


	//motor_control_pwm(999,999);
	while(1)
	{
		loop_states();
	}

	while(1)
	{

	}

	return 0;
}
