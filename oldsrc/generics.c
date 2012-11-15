/*
 * generics.c
 *
 *  Created on: Oct 10, 2012
 *      Author: alexs
 */


int int_to_a(char * str, int number)
{

	if (number > 0)
	{
		int i;
		int pos = 0;
		int digit;
		int digitfound = 0;
		const int units[] =
		{ 1000000000,
				100000000,
				10000000,
				1000000,
				100000,
				10000,
				1000,
				100,
				10,
				1};
		int unit;



		for (i = 0; i < 10; ++i)
		{
			unit  = units[i];
			digit = 0;

			while (number >= unit)
			{
				digit++;
				number -= unit;
			}

			if (digit != 0) digitfound = 1;
			if (digitfound) str[pos++] = digit + '0' ;
		}
		str[pos] = '\0';
	}
	else if (number == 0)
	{
		str[0] = '0';
		str[1] = '\0';
	}
	else
	{
		str[0] = '-';
		int_to_a(str+1, -number);
	}

	return 0;
}



void delay(u32 nCount)
{
  for(; nCount != 0; nCount--);
}
