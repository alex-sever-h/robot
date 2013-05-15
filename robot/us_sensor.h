/*
 * us_sensor.h
 *
 *  Created on: Oct 2, 2012
 *      Author: alexs
 */

#ifndef US_SENSOR_H_
#define US_SENSOR_H_

#include <libopencm3/stm32/f1/gpio.h>

#define RIGHT 1
#define LEFT -1

#define US_SENSOR_GPIO		GPIOA

#define US_SENSOR_TRIG		GPIO4

#define US_SENSOR_R_L		GPIO5
#define US_SENSOR_R_R		GPIO6
#define US_SENSOR_F_L		GPIO7
#define US_SENSOR_F_R		GPIO8

#define US_SENSOR_ECHO US_SENSOR_F_L | US_SENSOR_F_R | US_SENSOR_R_L | US_SENSOR_R_R

extern volatile unsigned int f_left_distance;
extern volatile unsigned int f_right_distance;
extern volatile unsigned int r_left_distance;
extern volatile unsigned int r_right_distance;


extern volatile int up_fl;
extern volatile int up_fr;
extern volatile int up_rl;
extern volatile int up_rr;


void check_us_sensor();
void us_sensor_config(void);
void us_sensor_trigger();


#endif /* US_SENSOR_H_ */
