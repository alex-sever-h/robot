/*
 * us_sensor.h
 *
 *  Created on: Oct 2, 2012
 *      Author: alexs
 */

#ifndef US_SENSOR_H_
#define US_SENSOR_H_


#define RIGHT 1
#define LEFT -1

extern unsigned int f_left_distance;
extern unsigned int f_right_distance;


void check_us_sensor();
void us_sensor_config(void);
void us_sensor_trigger();


#endif /* US_SENSOR_H_ */
