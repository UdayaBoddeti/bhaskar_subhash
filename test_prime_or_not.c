// Test prime or not

#include <stdio.h>
#include "check_prime_or_not.c"

void main()
{
	char test_values[8][8] = {"0", "123", "17", "190", "145", "10Ab", "ab10", "ab"};
	int result[8] = {-1, 0, 1, 0, 0, 0, -1, -1};
	int count = 8, counter;

	for (counter = 0;counter < count;counter++)
	{
		if (is_prime(test__values[counter]) != result[counter])
		{
			printf("%s: Failed.\n", test_values[counter]);
		}
	}
}
