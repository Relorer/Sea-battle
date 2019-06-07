#include "pch.h"
#include "randset.h"

void swap(int & a, int & b)
{
	a += b;
	b = a - b;
	a = a - b;
}

int generateRandomInTheRange(int min, int max)
{
	if (min > max) swap(min, max);
	return rand() % (++max - min) + min;
}
