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
//#include "bluetooth.h"
//#include "us_sensor.h"
#include "motor_ctrl.h"
//#include "pin_config.h"


void delay(u32 nCount);

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

	usart_init(921600);
	bt_init(460800);

	motor_GPIO_config();
	motor_TIMER_config();

	motor_control_pwm(00,00);

	usart_puts("ana are mere\r");

	gpio_set_mode(GPIOA,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_PUSHPULL,
			GPIO2 | GPIO3);

	gpio_set(GPIOA, GPIO2 | GPIO3);

	delay(10000);
	bt_puts("AT\r");


	while(1) {
		u8 c;
		gpio_toggle(GPIOA, GPIO2 | GPIO3);

		if((c = usart1_get_noblock()))
			bt_put(c);

		if((c = bt_get_noblock()))
		{
			bt_connected_status();
		}
	}



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

	return 0;
}


void delay(u32 nCount)
{
	u32 i;
	for(i = 0 ; i < nCount; i++)
		__asm__("nop");
}

