#include "LN.h"

#include <cstdio>
#include <iostream>

///--------------------------------NOT AN ACTIVE PROJECT FILE--------------------------------///

// LN ln_dvide(LN &first, LN &second)
//{
//	first.print();
//	printf("\n");
//	second.print();
//	printf("\n");
//
//	LN result((long long)((1.0 / (second.digits[second.n-2] - '0')) * 10));
//	if(result.n == 3)
//	{
//		result.digits[0] = '1';
//		result.end_number(1);
//	}
//
//	printf("result: ");
//	result.print();
//	printf("\n");
//
//
//	size_t required_number_digits = first.n - second.n + 3;
//	LN two(0ll);
//
//	LN *prev = new LN(first * second);
//
//	while(true)
//	{
//		size_t n = result.n - 1 + second.n - 1;
//		two.end_number(n);
//		two.digits[--n] = '2';
//		while(n--)
//			two.digits[n] = '0';
//		two.print();
//		printf("\n");
//
//		two -= LN(result * second);
//
//		two.print();
//		printf("\n");
//
//		result *= two;
//
//		printf("result = ");
//		result.print();
//		printf("\n");
//
//		if(result.n > required_number_digits)
//		{
//			for (size_t i = 0, shift = result.n - required_number_digits; i < required_number_digits; i++)
//			{
//				result.digits[i] = result.digits[i + shift];
//			}
//			result.reallocate(required_number_digits);
//			result.print();
//			printf("\n");
//		}
//
//		LN *next = new LN(first * result);
//		printf("next = ");
//		next->print();
//		printf("\n");
//		if(next->n >= required_number_digits)
//		{
//			bool eq = true;
//			for(size_t i = 1; i <= required_number_digits; i++)
//			{
//				if(prev->digits[prev->n - 1 - i] != next->digits[next->n - 1 - i])
//				{
//					eq = false;
//					break;
//				}
//			}
//			if(eq)
//			{
//				delete prev;
//				prev = next;
//				break;
//			}
//		}
//		delete prev;
//		prev = next;
//	}
//
//	printf("res = \t\t");
//	prev->print();
//	printf("\n");
//
//	LN result_dv(*prev);
//	delete prev;
//
//	result_dv.leave_end(result_dv.n - result.n - second.n + 3);
//
//	printf("res dv = \t");
//	result_dv.print();
//	printf("\n");
//
//
//	return result_dv;
// }

long long long_long_dvide(long long first, long long second)
{
	long long first_dig = second;
	long long m = 2;
	while (first_dig > 100)
	{
		first_dig /= 10;
		m++;
	}

	double dd = 1.0 / first_dig;

	long long d = dd * 100;

	long long n = 10;
	long long first_buf = first;
	while (first_buf > 10)
	{
		first_buf /= 10;
		n *= 10;
	}

	std::cout << "first " << first_dig << " " << m << " " << d << std::endl;

	int eps = 10;

	for (;;)
	{
		// int mm = second * d;
		long long tt = 2;

		long long res_d = d;

		while (res_d > 1)
		{
			res_d /= 10;
			tt *= 10;
		}
		for (int j = 0; j < m - 1; j++)
		{
			tt *= 10;
		}

		std::cout << "old d= " << tt << " " << second * d << " " << tt - second * d << " " << d << std::endl;
		d = d * (tt - second * d);

		while (d > n)
		{
			d /= 10;
		}

		std::cout << "new d= " << d << std::endl;

		long long res = second * d;

		while (res > n)
		{
			res /= 10;
		}

		std::cout << "res= " << res << std::endl;
		if (res - n < eps && res - n > -eps)
			break;
	}

	long long res = first * d;

	std::cout << "res= " << res << std::endl;

	while (d > 1)
	{
		res /= 10;
		d /= 10;
	}
	for (int j = 0; j < m - 1; j++)
	{
		res /= 10;
	}

	std::cout << "res= " << res << std::endl;

	return res;
}

double double_dvide(double first, double second)
{
	double d = 0.05;

	double eps = 0.01;

	while ((first * d) * second - first > eps || (first * d) * second - first < -eps)
	{
		d = d * (2 - second * d);
		std::cout << "result = " << d << std::endl;
	}

	std::cout << "a = " << first << " b = " << second << " 1/b = " << d << " a/b = " << first * d
			  << " real a/b = " << first / second << std::endl;

	return first * d;
}

///--------------------------------NOT AN ACTIVE PROJECT FILE--------------------------------///