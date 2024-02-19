// Test prime or not

#include <stdio.h>
#include "check_prime_or_not.c"

void main()
{
	char test_values[10][10] = {"0", "123", "17", "190", "145", "10Ab", "ab10", "ab"};
	int count = 8, counter;

	for (counter = 0;counter < count;counter++)
	{
		int prime_or_not = is_prime(test_values[counter]);

		if (prime_or_not == 1)
		{
			printf("%d is a prime number.\n", atoi(test_values[counter]));
		}
		else if (prime_or_not == -1)
		{
			printf("%s is invalid input.\n", test_values[counter]);
		}
		else
		{
			printf("%d is not a prime number.\n", atoi(test_values[counter]));
		}
	}
}