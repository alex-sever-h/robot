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

void bt_put(uint8_t ch);
uint8_t bt_get(void);

void bt_puts(const u8 * string);



#endif /* BLUETOOTH_H_ */
