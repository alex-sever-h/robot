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
//#include "us_sensor.h"
#include "motor_ctrl.h"
//#include "pin_config.h"



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
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_USART1EN);

	rcc_peripheral_enable_clock(&RCC_APB1ENR,
			RCC_APB1ENR_USART3EN |
			RCC_APB1ENR_TIM3EN );
}


int main(void)
{
	clock_setup();

	//init peripherals
	usart_init(921600);
	bt_init(460800);
	motor_init();


	usart_puts("...... UART TEST ......\r");

	gpio_set_mode(GPIOA,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_PUSHPULL,
			GPIO2 | GPIO3);

	gpio_clear(GPIOA, GPIO2 | GPIO3);


	bt_wait_connected_status();



	while(1) {
		char c;

		c = bt_get_block();

		interpret_byte(c);

		//		switch(c)
		//		{
		//		case 'f':
		//		{
		//			motor_control_pwm(1000,1000);
		//		}break;
		//		case 'l':
		//		{
		//			motor_control_pwm(300,1000);
		//		}break;
		//		case 'r':
		//		{
		//			motor_control_pwm(1000, 300);
		//		}break;
		//		case 'F':
		//		{
		//			motor_control_pwm(-1000,-1000);
		//		}break;
		//		case 'L':
		//		{
		//			motor_control_pwm(-300,-1000);
		//		}break;
		//		case 'R':
		//		{
		//			motor_control_pwm(-1000, -300);
		//		}break;
		//		default:
		//		{
		//			motor_control_pwm(0, 0);
		//		}break;
		//		}
		//
		//


		//
		//
		//	//	init_counter_sensor();
		//	//	us_sensor_config();
		//
		//
		//	usart_puts("test ...........");
		//
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//
		//
		//	bt_puts("AT\r");
		//
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//	Delay(0xFFFF);
		//
		//
		//
		//	char c;
		//	while(1)
		//	{
		//		//		c = bt_get();
		//		//		if ( c != 3 )
		//		//			usart1_put(c);
		//		//
		//		//		c = usart1_get();
		//		//		if ( c != 3)
		//		//			bt_put(c);
		//
		//		Delay(0xFFFF);
		//		bt_send_sensor_reading('5', 43245);
		//
		//	}
		//
		//
		//	motor_GPIO_config();
		//	motor_TIMER_config();
		//
		//
		//	motor_control_pwm(0,0);
		//
		//
		//
		//	while (1)
		//	{
		//

		//		if (f_left_distance < 400 || f_right_distance < 400)
		//		{
		//			if (f_left_distance < 150 || f_right_distance < 150)
		//			{
		//				if (f_left_distance < 50 || f_right_distance < 50)
		//				{
		//					motor_stop();
		//				}
		//				else
		//				{
		//					if ( f_right_distance < f_left_distance)
		//						motor_control_pwm(-999, -300);
		//					else
		//						motor_control_pwm(-300, -999);
		//				}
		//
		//			}
		//			else
		//			{
		//				if ( f_right_distance < f_left_distance)
		//					motor_control_pwm(300, 900);
		//
		//				else
		//					motor_control_pwm(900, 300);
		//			}
		//		}
		//		else
		//		{
		//			motor_go_forward(900);
		//		}
		//
		//
		//
		//
		//		if(n%2)
		//			us_sensor_trigger(RIGHT);
		//		else
		//			us_sensor_trigger(LEFT);
		//
		//		n++;
		//		Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);
		//		Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);
		//		Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);
		//		Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);Delay(0xAFFF);
		//	}


	}

	return 0;
}
