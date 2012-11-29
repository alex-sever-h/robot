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
	//buffer->count = 0;
	buffer->in    = 0;
	buffer->out   = 0;
}

inline u32 rb_inc_index(u32 index)
{
	index++;
	if(index == USARTBUFFSIZE)
		index = 0;
	return index;
}

inline u32 rb_dec_index(u32 index)
{
	index--;
	if(index == -1)
		index = USARTBUFFSIZE-1;
	return index;
}

int buffer_put(volatile RINGBUFFER *buffer, u8 c)
{
	if (buffer->in == rb_dec_index(buffer->out) )
	{
		buffer->out = rb_inc_index(buffer->out);
	}

	buffer->data[buffer->in]=c;
	//buffer->count++;

	buffer->in = rb_inc_index(buffer->in);

	return SUCCESS;
}

int buffer_get(volatile RINGBUFFER *buffer, u8 *c)
{
	if (buffer->in == buffer->out)
		return FAILURE;

	*c = buffer->data[buffer->out];
	//buffer->count--;

	buffer->out = rb_inc_index(buffer->out);


	return SUCCESS;
}

int buffer_empty(volatile RINGBUFFER *buffer)
{
	if (buffer->in == buffer->out)
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



int buffer_get_last_line(volatile RINGBUFFER *buffer, char * string)
{
	int startpos;
	int first_len;
	int last_len;
	int i, j, offset;


	for(i = buffer->in-1;(buffer->data[i] != '\r') || (i != buffer->in); i--)
	{
		if(i < 0) i = USARTBUFFSIZE;
	}

	for(j = 0; i <= buffer->in; i++, j++)
	{
		string[j] = buffer->data[i];
	}
	buffer->out = i;

	return SUCCESS;
}


































