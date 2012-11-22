/*
 * rbuffer.h
 *
 *  Created on: Apr 12, 2012
 *      Author: alexs
 */

#ifndef RBUFFER_H_
#define RBUFFER_H_

#include <stdint.h>
#include <libopencm3/cm3/common.h>

#define FAILURE 0
#define SUCCESS 1

#define USARTBUFFSIZE 1024

typedef struct{
  u32 in;
  u32 out;
  u32 count;
  u8 data[USARTBUFFSIZE];
}RINGBUFFER;


 void buffer_reset  (volatile RINGBUFFER *buffer);
 int  buffer_put    (volatile RINGBUFFER *buffer, u8  c);
 int  buffer_get    (volatile RINGBUFFER *buffer, u8 *c);
 int  buffer_empty  (volatile RINGBUFFER *buffer);




#endif /* RBUFFER_H_ */
