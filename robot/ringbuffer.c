/*
 * rbuffer.c
 *
 *  Created on: Apr 12, 2012
 *      Author: alexs
 */

#include <string.h>
#include "ringbuffer.h"

void buffer_reset(volatile RINGBUFFER *buffer)
{
	buffer->count = 0;
	buffer->in    = 0;
	buffer->out   = 0;
}

int buffer_put(volatile RINGBUFFER *buffer, u8 c)
{
	if  (buffer->count == USARTBUFFSIZE)
		return FAILURE;

	buffer->data[buffer->in++]=c;
	buffer->count++;

	if (buffer->in == USARTBUFFSIZE)
		buffer->in=0;

	return SUCCESS;
}

int buffer_get(volatile RINGBUFFER *buffer, u8 *c)
{
	if (buffer->count == 0)
		return FAILURE;

	*c = buffer->data[buffer->out++];
	buffer->count--;

	if (buffer->out == USARTBUFFSIZE)
		buffer->out = 0;

	return SUCCESS;
}

int buffer_empty(volatile RINGBUFFER *buffer)
{
	if (buffer->count == 0)
		return SUCCESS;
	else
		return FAILURE;

}

int buffer_strncpy(volatile RINGBUFFER *buffer, char * string, int n)
{
	int startpos;
	int first_len;
	int last_len;
	int i, offset;

	startpos = buffer->out - n;
	if(startpos < 0)
	{
		first_len = -startpos;
		last_len  = n - first_len;
		startpos = USARTBUFFSIZE + startpos;
	}
	else
	{
		first_len = n;
		last_len  = 0;
	}

	for(i = 0, offset = startpos ; i < n; ++i, ++offset)
	{
		if(offset == USARTBUFFSIZE)
			offset = 0;

		string[i] = buffer->data[offset];
	}

	//	strncpy(string, buffer->data + startpos , first_len);
	//
	//	strncpy(string+first_len, buffer->data, last_len);

	return SUCCESS;
}


































