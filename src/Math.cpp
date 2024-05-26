#include "Math.h"

unsigned long Math::factorial(unsigned int n)
{
	unsigned int value = 1;

	for (unsigned int i = 2; i <= n; i++)
	{
		value *= i;
	}

	return value;
}

double Math::pow(double x, unsigned int n)
{
	double raised = 1;

	for (unsigned int i = 1; i <= n; i++)
	{
		raised *= x;
	}

	return raised;
}

unsigned int Math::pow(unsigned int x, unsigned int n)
{
	unsigned int raised = 1;

	for (unsigned int i = 1; i <= n; i++)
	{
		raised *= x;
	}

	return raised;
}
