/*
 * bluetooth.h
 *
 *  Created on: Nov 6, 2012
 *      Author: alexs
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define BUFFERED


void bt_init( int baudrate );

void bt_put(char ch);
char bt_get(void);

void bt_puts(const char * string);

int bt_check_already_connected();

#endif /* BLUETOOTH_H_ */
