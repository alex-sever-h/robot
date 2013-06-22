/*
 * motor_ctrl.c
 *
 *  Created on: Oct 10, 2012
 *      Author: alexs
 */
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <libopencm3/stm32/timer.h>
#include "motor_ctrl.h"

#define MOTOR_RESOLUTION 500

static u16 timer_period;
static u16 motor_front_left;
static u16 motor_rear_left;
static u16 motor_front_rigth;
static u16 motor_rear_rigth;


static inline u16 get_oc_value(u32 percet, u32 percentage)
{
	return  (( (u32) percet * (timer_period - 1)) / percentage);
}

void motor_init()
{
	motor_GPIO_config();
	motor_PWM_TIMER_config();
	motor_DELAY_TIMER_config();
	motor_control_pwm(0, 0);
}

void motor_GPIO_config(void)
{
	/* PWM pins */
	gpio_set_mode(GPIOC,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
			MOTOR_GPIO_PWM_FRONT_L |
			MOTOR_GPIO_PWM_FRONT_R |
			MOTOR_GPIO_PWM_REAR_L |
			MOTOR_GPIO_PWM_REAR_R );

	// remap TIM3 to PCx pins
	gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ, AFIO_MAPR_TIM3_REMAP_FULL_REMAP);

	/* direction pins*/
	gpio_set_mode(GPIOC,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_PUSHPULL,
			MOTOR_DIR_L_1 |
			MOTOR_DIR_L_2 |
			MOTOR_DIR_R_1 |
			MOTOR_DIR_R_2);

	gpio_clear(GPIOC, MOTOR_DIR_L_1 | MOTOR_DIR_L_2 | MOTOR_DIR_R_1 | MOTOR_DIR_R_2);

}

void motor_control_pwm(int left, int right)
{
	//set direction
	if (left == 0)
	{
		gpio_clear(GPIOC, MOTOR_DIR_L_1 | MOTOR_DIR_L_2);

	}
	else if (left < 0)
	{
		gpio_set  (GPIOC, MOTOR_DIR_L_1);
		gpio_clear(GPIOC, MOTOR_DIR_L_2);
		left = -left;
	}
	else
	{
		gpio_set  (GPIOC, MOTOR_DIR_L_2);
		gpio_clear(GPIOC, MOTOR_DIR_L_1);
	}

	//set direction
	if (right == 0)
	{
		gpio_clear(GPIOC, MOTOR_DIR_R_1 | MOTOR_DIR_R_2);

	}
	else if (right < 0)
	{
		gpio_set  (GPIOC, MOTOR_DIR_R_1);
		gpio_clear(GPIOC, MOTOR_DIR_R_2);
		right = -right;
	}
	else
	{
		gpio_set  (GPIOC, MOTOR_DIR_R_2);
		gpio_clear(GPIOC, MOTOR_DIR_R_1);
	}

	// calculate compare registers
	motor_front_left  = get_oc_value( left  , MOTOR_RESOLUTION);
	motor_rear_left   = get_oc_value( left  , MOTOR_RESOLUTION);
	motor_front_rigth = get_oc_value( right , MOTOR_RESOLUTION);
	motor_rear_rigth  = get_oc_value( right , MOTOR_RESOLUTION);

	// Set the capture compare value
	timer_set_oc_value(TIM3, MOTOR_PWM_FRONT_LEFT,  motor_front_left);
	timer_set_oc_value(TIM3, MOTOR_PWM_REAR_LEFT,   motor_rear_left);
	timer_set_oc_value(TIM3, MOTOR_PWM_FRONT_RIGHT, motor_front_rigth);
	timer_set_oc_value(TIM3, MOTOR_PWM_REAR_RIGHT,  motor_rear_rigth);

}

int motor_run_time(int time_ms)
{
	if(time_ms >= 0)
		motor_control_pwm(+450, +450);
	else
		motor_control_pwm(-450, -450);

	bt_puts("ACK\n");

	motor_start_delay(abs(time_ms));

	return 1;
}

int motor_rot_time(int time_ms)
{
	if(time_ms >= 0)
		motor_control_pwm(-450, +450);
	else
		motor_control_pwm(+450, -450);

	bt_puts("ACK\n");

	motor_start_delay(abs(time_ms));

	return 1;
}

/** Directly control motor left and right PWM in 128 steps
 */
void motor_control_lr(int left, int right)
{
	int left_pwm;
	int right_pwm;

	left_pwm = MOTOR_RESOLUTION * left / 128;
	right_pwm = MOTOR_RESOLUTION * right / 128;

	motor_control_pwm(left_pwm, right_pwm);
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

int motor_DELAY_TIMER_config(void)
{
	nvic_enable_irq(NVIC_TIM4_IRQ);

	timer_reset(TIM4);

	/* set timer mode  no divider ; alignment on edge ; direction up */
	timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);


	int timer_prescaler = rcc_ppre1_frequency / 1000;

	timer_set_prescaler(TIM4, timer_prescaler);
	timer_set_period(TIM4, 2000);
	timer_set_repetition_counter(TIM4, 0);
	timer_disable_preload(TIM4);

	//timer_one_shot_mode(TIM4);

	timer_enable_irq(TIM4, TIM_DIER_UIE);

	return 1;
}

int motor_start_delay(int timeMs)
{
	timer_set_period(TIM4, timeMs*2);
	timer_enable_counter(TIM4);

	gpio_set(GPIOA, GPIO2 | GPIO3);
}


void tim4_isr(void)
{
	timer_clear_flag(TIM4, TIM_DIER_UIE);
	timer_disable_counter(TIM4);

	motor_stop();

	bt_puts("RDY\n");

	gpio_clear(GPIOA, GPIO2 | GPIO3);
}

void motor_PWM_TIMER_config(void)
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

	motor_front_left  = 0;
	motor_rear_left   = 0;
	motor_front_rigth = 0;
	motor_rear_rigth  = 0;

	/************ channels configuration ************/

	timer_disable_oc_output(TIM3, MOTOR_PWM_REAR_LEFT);
	timer_disable_oc_output(TIM3, MOTOR_PWM_FRONT_LEFT);
	timer_disable_oc_output(TIM3, MOTOR_PWM_REAR_RIGHT);
	timer_disable_oc_output(TIM3, MOTOR_PWM_FRONT_RIGHT);

	//configure OCx line
	timer_set_oc_slow_mode(TIM3, MOTOR_PWM_REAR_LEFT);
	timer_set_oc_slow_mode(TIM3, MOTOR_PWM_FRONT_LEFT);
	timer_set_oc_slow_mode(TIM3, MOTOR_PWM_REAR_RIGHT);
	timer_set_oc_slow_mode(TIM3, MOTOR_PWM_FRONT_RIGHT);

	timer_set_oc_mode(TIM3, MOTOR_PWM_REAR_LEFT, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM3, MOTOR_PWM_FRONT_LEFT, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM3, MOTOR_PWM_REAR_RIGHT, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM3, MOTOR_PWM_FRONT_RIGHT, TIM_OCM_PWM1);

	//configure OCx output
	timer_set_oc_polarity_high(TIM3, MOTOR_PWM_REAR_LEFT);
	timer_set_oc_polarity_high(TIM3, MOTOR_PWM_FRONT_LEFT);
	timer_set_oc_polarity_high(TIM3, MOTOR_PWM_REAR_RIGHT);
	timer_set_oc_polarity_high(TIM3, MOTOR_PWM_FRONT_RIGHT);

	timer_set_oc_idle_state_set(TIM3, MOTOR_PWM_REAR_LEFT);
	timer_set_oc_idle_state_set(TIM3, MOTOR_PWM_FRONT_LEFT);
	timer_set_oc_idle_state_set(TIM3, MOTOR_PWM_REAR_RIGHT);
	timer_set_oc_idle_state_set(TIM3, MOTOR_PWM_FRONT_RIGHT);

	/* Set the capture compare value */
	timer_set_oc_value(TIM3, MOTOR_PWM_FRONT_LEFT,  motor_front_left);
	timer_set_oc_value(TIM3, MOTOR_PWM_REAR_LEFT,   motor_rear_left);
	timer_set_oc_value(TIM3, MOTOR_PWM_FRONT_RIGHT, motor_front_rigth);
	timer_set_oc_value(TIM3, MOTOR_PWM_REAR_RIGHT,  motor_rear_rigth);


	timer_enable_oc_output(TIM3, MOTOR_PWM_REAR_LEFT);
	timer_enable_oc_output(TIM3, MOTOR_PWM_FRONT_LEFT);
	timer_enable_oc_output(TIM3, MOTOR_PWM_REAR_RIGHT);
	timer_enable_oc_output(TIM3, MOTOR_PWM_FRONT_RIGHT);

	timer_enable_counter(TIM3);

}
