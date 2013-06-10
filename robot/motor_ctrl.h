/*
 * motor_ctrl.h
 *
 *  Created on: Oct 10, 2012
 *      Author: alexs
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/timer.h>


#define MOTOR_GPIO      	GPIOC

#define MOTOR_GPIO_PWM_REAR_R	GPIO6
#define MOTOR_GPIO_PWM_FRONT_R	GPIO7
#define MOTOR_GPIO_PWM_REAR_L	GPIO8
#define MOTOR_GPIO_PWM_FRONT_L	GPIO9


#define MOTOR_DIR_L_1		GPIO4
#define MOTOR_DIR_L_2		GPIO5
#define MOTOR_DIR_R_1		GPIO10
#define MOTOR_DIR_R_2		GPIO11

#define MOTOR_PWM_REAR_LEFT		TIM_OC3
#define MOTOR_PWM_FRONT_LEFT	TIM_OC4
#define MOTOR_PWM_REAR_RIGHT	TIM_OC1
#define MOTOR_PWM_FRONT_RIGHT	TIM_OC2

void motor_init(void);

void motor_control_lr(int left, int right);

#endif /* MOTOR_CTRL_H_ */
