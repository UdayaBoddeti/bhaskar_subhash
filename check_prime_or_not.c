// Check whether a given number is prime or not

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int is_prime(char number[])
{
	int numeric_value;
	numeric_value = atoi(number);
	if (numeric_value == 0)
	{
		return -1;
	}
	else
	{
		int root = sqrt(numeric_value);
		if (numeric_value == 2)
		{
			return 1;
		}
		else if (numeric_value < 2 || numeric_value % 2 == 0)
		{
			return 0;
		}
		else
		{
			int counter = 3;
			while (counter <= root)
			{
				if (numeric_value % counter == 0)
				{
					return 0;
				}
				counter = counter + 2;
			}
			return 1;
		}
	}
}