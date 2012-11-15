/*
 * motor_ctrl.c
 *
 *  Created on: Oct 10, 2012
 *      Author: alexs
 */
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/timer.h>
#include "motor_ctrl.h"

static u16 timer_period;
static u16 motor_front_left;
static u16 motor_rear_left;
static u16 motor_front_rigth;
static u16 motor_rear_rigth;


inline u16 get_oc_value(u32 percet, u32 percentage)
{
	return  (( (u32) percet * (timer_period - 1)) / percentage);
}

void motor_init()
{
	motor_GPIO_config();
	motor_TIMER_config();
	motor_control_pwm(0, 0);
}

void motor_GPIO_config(void)
{
	/* PWM pins */
	gpio_set_mode(GPIOC,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
			GPIO_TIM3_FR_CH1 |
			GPIO_TIM3_FR_CH2 |
			GPIO_TIM3_FR_CH3 |
			GPIO_TIM3_FR_CH4 );

	// remap TIM3 to PCx pins
	gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ, AFIO_MAPR_TIM3_REMAP_FULL_REMAP);

	/* direction pins*/
	gpio_set_mode(GPIOC,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_PUSHPULL,
			GPIO10 |
			GPIO11 );

	gpio_clear(GPIOC, GPIO10 | GPIO11);

}



void motor_control_pwm(int left, int right)
{
	//set direction
	if (left < 0)
	{
		gpio_set(GPIOC, GPIO10);
		left = 1000 + left;
	}
	else
	{
		gpio_clear(GPIOC, GPIO10);
	}

	if (right < 0)
	{
		gpio_set(GPIOC, GPIO11);
		right = 1000 + right;
	}
	else
	{
		gpio_clear(GPIOC, GPIO11);
	}

	// calculate compare registers
	motor_front_left = get_oc_value( left  , 1000);
	motor_rear_left = get_oc_value( left  , 1000);
	motor_front_rigth = get_oc_value( right , 1000);
	motor_rear_rigth = get_oc_value( right , 1000);

	// Set the capture compare value
	timer_set_oc_value(TIM3, TIM_OC1, motor_front_left);
	timer_set_oc_value(TIM3, TIM_OC2, motor_rear_left);
	timer_set_oc_value(TIM3, TIM_OC3, motor_front_rigth);
	timer_set_oc_value(TIM3, TIM_OC4, motor_rear_rigth);

}



void motor_go_forward(int speed)
{
	motor_control_pwm(speed, speed);
}

void motor_go_backwards(int speed)
{
	motor_control_pwm(-speed, -speed);
}

void motor_turn_left()
{

}

void motor_turn_right()
{

}

void motor_rotate_left(int speed)
{
	motor_control_pwm(-speed, +speed);
}

void motor_rotate_right(int speed)
{
	motor_control_pwm(+speed, -speed);
}

void motor_stop()
{
	motor_control_pwm(0,0);
}



void motor_panic_stop()
{

	gpio_clear(GPIOC, GPIO10 | GPIO11);

}


void motor_TIMER_config(void)
{

	timer_period = rcc_ppre1_frequency / 5000;

	timer_reset(TIM3);
	/* set timer mode  no divider ; alignment on edge ; direction up */
	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_continuous_mode(TIM3);
	/* Period (5kHz). TIM3 clk source is APB1*/
	timer_set_period(TIM3, timer_period);
	/* Reset prescaler value. */
	timer_set_prescaler(TIM3, 0);

	motor_front_left  = get_oc_value( 0 , 1000);
	motor_rear_left   = get_oc_value( 0 , 1000);
	motor_front_rigth = get_oc_value( 0 , 1000);
	motor_rear_rigth  = get_oc_value( 0 , 1000);

	/************ channels configuration ************/

	timer_disable_oc_output(TIM3, TIM_OC1);
	timer_disable_oc_output(TIM3, TIM_OC2);
	timer_disable_oc_output(TIM3, TIM_OC3);
	timer_disable_oc_output(TIM3, TIM_OC4);

	//configure OCx line
	timer_set_oc_slow_mode(TIM3, TIM_OC1);
	timer_set_oc_slow_mode(TIM3, TIM_OC2);
	timer_set_oc_slow_mode(TIM3, TIM_OC3);
	timer_set_oc_slow_mode(TIM3, TIM_OC4);

	timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM3, TIM_OC4, TIM_OCM_PWM1);

	//configure OCx output
	timer_set_oc_polarity_high(TIM3, TIM_OC1);
	timer_set_oc_polarity_high(TIM3, TIM_OC2);
	timer_set_oc_polarity_high(TIM3, TIM_OC3);
	timer_set_oc_polarity_high(TIM3, TIM_OC4);

	timer_set_oc_idle_state_set(TIM3, TIM_OC1);
	timer_set_oc_idle_state_set(TIM3, TIM_OC2);
	timer_set_oc_idle_state_set(TIM3, TIM_OC3);
	timer_set_oc_idle_state_set(TIM3, TIM_OC4);

	/* Set the capture compare value */
	timer_set_oc_value(TIM3, TIM_OC1, motor_front_left);
	timer_set_oc_value(TIM3, TIM_OC2, motor_rear_left);
	timer_set_oc_value(TIM3, TIM_OC3, motor_front_rigth);
	timer_set_oc_value(TIM3, TIM_OC4, motor_rear_rigth);


	timer_enable_oc_output(TIM3, TIM_OC1);
	timer_enable_oc_output(TIM3, TIM_OC2);
	timer_enable_oc_output(TIM3, TIM_OC3);
	timer_enable_oc_output(TIM3, TIM_OC4);

	timer_enable_counter(TIM3);

}
