#include "LN.h"

#include <climits>

//#ifdef TEST
//#	if TEST == 0
//#		define PRINT
//#	endif
//#endif

#define MIN(x, y) (x > y ? (y) : (x))
#define MAX(x, y) (x < y ? (y) : (x))

LN operator"" _ln(const char *str)
{
	return LN(str);
}

LN::LN(const LN &copied) :
	n(copied.n), digits((char *)malloc(copied.n * sizeof(char))), isNegative(copied.isNegative), isNaN(copied.isNaN)
{
	if (!digits)
		throw ERROR_OUTOFMEMORY;
	for (size_t i = 0; i < n; i++)
		digits[i] = copied.digits[i];
}

LN::LN(LN &&movable) noexcept :
	n(movable.n), digits(movable.digits), isNegative(movable.isNegative), isNaN(movable.isNaN)
{
	movable.digits = nullptr;
}

LN::LN(const char *const number)
{
	if (!digits)
		throw ERROR_OUTOFMEMORY;

	size_t i = 0;
	size_t j = 0;

	while (number[i++] != '\0')
		;

	reallocate(i);
	i--;
	while (i--)
		digits[j++] = number[i];
	isNegative = false;

	digits[j] = '\0';
}

LN::LN(const char *digits, size_t n) : digits((char *)digits), n(n), isNegative(false) {}

LN::LN(const char *new_digits, size_t new_n, bool isCopy) : isNegative(false)
{
	if (!digits)
		throw ERROR_OUTOFMEMORY;

	digits = (char *)malloc((new_n + 1) * sizeof(char));
	if (!digits)
		throw ERROR_OUTOFMEMORY;

	size_t i = 0;
	for (; i < new_n; i++)
	{
		digits[i] = new_digits[i];
	}
	while (--i)
	{
		if (digits[i] != '0')
			break;
	}
	i++;
	end_number(i);
}

LN::LN(const std::string_view number)
{
	if (!digits)
		throw ERROR_OUTOFMEMORY;

	size_t i = 0;
	size_t j = 0;

	while (number[i++] != '\0')
		;

	reallocate(i);
	i--;
	while (i--)
		digits[j++] = number[i];
	isNegative = false;

	digits[j] = '\0';
}

LN::LN(const long long number)
{
	if (!digits)
		throw ERROR_OUTOFMEMORY;

	unsigned long long value;
	if (number < 0)
	{
		isNegative = true;
		value = number * -1;
	}
	else
	{
		isNegative = false;
		value = number;
	}

	if (value == 0)
	{
		digits[0] = '0';
		end_number(1);
		return;
	}

	size_t i = 0;
	while (value > 0)
	{
		digits[i++] = (char)(value % 10 + '0');

		value /= 10;
	}
	end_number(i);
}

LN::operator long long() const
{
	if (n > 20 || isNaN)
		throw ERROR_INVALID_DATA;

	if (n == 20)
	{
		if (isNegative)
		{
			LN ln_min((long long)(LLONG_MIN));

			if (comparison(ln_min) == more)
				throw ERROR_INVALID_DATA;
		}
		else
		{
			LN ln_max((long long)LLONG_MAX);

			if (comparison(ln_max) == more)
				throw ERROR_INVALID_DATA;
		}
	}

	long long result = 0;
	long long multiplier = 1;

	for (size_t i = 0; i < n - 1; i++, multiplier *= 10)
		result += (digits[i] - '0') * multiplier;

	if (isNegative)
		result *= -1;

	return result;
}

bool LN::scan(FILE *in)
{
	char d;
	if (fscanf(in, "%c", &d) != EOF)
	{
		if (d >= '0' && d <= '9')
		{
			size_t size = 0;
			if (d != '0')
				digits[size++] = d;
			else
			{
				while (fscanf(in, "%c", &d) != EOF)
				{
					if (d == '0')
						continue;
					else if (d >= '1' && d <= '9')
						digits[size++] = d;
					else
						ungetc(d, in);
					break;
				}
				if (size == 0)
				{
					digits[size++] = '0';
					isNegative = false;
					end_number(size);
					revers();
					return true;
				}
			}

			while (fscanf(in, "%c", &d) != EOF)
			{
				if (d >= '0' && d <= '9')
				{
					add_digit(d, size++);
				}
				else
				{
					ungetc(d, in);
					break;
				}
			}

			if (size == 0)
				return false;

			end_number(size);
			revers();
			return true;
		}
		else
		{
			ungetc(d, in);
			return false;
		}
	}
	return false;
}
void LN::print_file(FILE *out) const noexcept
{
	if (isNegative)
		fprintf(out, "-");

	size_t i = n - 1;
	while (i--)
		fprintf(out, "%c", digits[i]);
}
[[maybe_unused]] void LN::print_str(char *out) const noexcept
{
	size_t j = 0;
	if (isNegative)
		out[j++] = '-';

	size_t i = n - 1;
	while (i--)
	{
		out[j++] = digits[i];
	}
	out[j] = '\0';
}

[[maybe_unused]] void LN::print() const noexcept
{
	if (isNegative)
		printf("-");

	size_t i = n - 1;
	while (i--)
		printf("%c", digits[i]);
}

[[nodiscard]] char *LN::get_str() const noexcept
{
	return digits;
}

void LN::reallocate()
{
	char *new_digits = (char *)realloc(digits, (n * 2) * sizeof(char));
	if (!new_digits)
	{
		new_digits = (char *)realloc(digits, (n + n / 2) * sizeof(char));
		if (!new_digits)
		{
			throw ERROR_OUTOFMEMORY;
		}
		n += n / 2;
	}
	else
		n *= 2;

	digits = new_digits;
}
void LN::reallocate(size_t size)
{
	if (size == n)
		return;

	char *new_digits = (char *)realloc(digits, (size) * sizeof(char));
	if (!new_digits)
		throw ERROR_OUTOFMEMORY;

	digits = new_digits;
	n = size;
}

void LN::add_digit(char d, size_t ind)
{
	if (ind + 1 == n)
		reallocate();

	digits[ind] = d;
}

void LN::end_number(size_t size)
{
	reallocate(size + 1);
	digits[size] = '\0';
}

void LN::revers()
{
	size_t beg = 0;
	size_t end = n - 2;
	while (beg < end)
	{
		char buf = digits[beg];
		digits[beg] = digits[end];
		digits[end] = buf;
		beg++;
		end--;
	}
}

void LN::plus(const LN &summand)
{
	// char remains = 0;
	bool remains = false;
	size_t i;

	for (i = 0; i < MIN(n, summand.n) - 1; i++)
	{
		digits[i] = (char)(digits[i] - '0' + summand.digits[i] + remains);
		if (digits[i] > '9')
		{
			digits[i] -= 10;
			remains = true;
		}
		else
			remains = false;
	}

	if (summand.n > n)
	{
		reallocate(summand.n);
		for (; i < summand.n - 1; i++)
		{
			digits[i] = (char)(summand.digits[i] + remains);
			if (digits[i] > '9')
			{
				digits[i] -= 10;
				remains = true;
			}
			else
				remains = false;
		}
		if (remains)
		{
			digits[i++] = '1';
		}
		end_number(i);
	}

	else
	{
		for (; i < n - 1 && remains; i++)
		{
			digits[i] = (char)(digits[i] + remains);
			if (digits[i] > '9')
			{
				digits[i] -= 10;
				remains = true;
			}
			else
				remains = false;
		}
		if (remains && i == n - 1)
		{
			digits[i++] = '1';
			end_number(i);
		}
	}
}

void LN::minus(const LN &second, const bool is_second_deductible)
{
	char remains = 0;
	size_t i;

	for (i = 0; i < MIN(n, second.n) - 1; i++)
	{
		if (is_second_deductible)
			digits[i] = (char)(digits[i] - (second.digits[i] - '0' + remains));
		else
			digits[i] = (char)(second.digits[i] - (digits[i] - '0' + remains));
		if (digits[i] < '0')
		{
			digits[i] += 10;
			remains = 1;
		}
		else
			remains = 0;
	}

	if ((!is_second_deductible) && second.n > n)
	{
		reallocate(second.n);
		for (; i < n - 1; i++)
		{
			digits[i] = (char)(second.digits[i] - remains);
			if (digits[i] < '0')
			{
				digits[i] += 10;
				remains = 1;
			}
			else
				remains = 0;
		}
	}
	else
	{
		for (; remains && i < n - 1; i++)
		{
			if (is_second_deductible)
				digits[i] = (char)(digits[i] - remains);
			else
				digits[i] = (char)(second.digits[i] - remains);
			if (digits[i] < '0')
			{
				digits[i] += 10;
				remains = 1;
			}
			else
				remains = 0;
		}
	}

	if (i == n - 1)
	{
		i--;
		while (digits[i] == '0' && i > 0)
			i--;

		end_number(i + 1);
	}
}

LN::Result_multiply_Karatsuba LN::multiply_Karatsuba(const char *one, const size_t n_one, const char *two, const size_t n_two)
{
	// printf("%zu %zu\n", n_one, n_two);

	if (n_one < 20 && n_two < 20)
	{
		char *new_digits = (char *)malloc((n_one + n_two + 2) * sizeof(char));
		if (!new_digits)
			throw ERROR_OUTOFMEMORY;

		unsigned int remains = 0;
		size_t i = 0;
		for (; i < n_one + n_two - 1; i++)
		{
			unsigned int num = 0;

			for (size_t j = ((i + 1) > (n_one)) ? (i + 1) - (n_one) : 0; j < MIN(n_two, i + 1); j++)
			{
				num += (one[i - j] - '0') * (two[j] - '0');
			}
			num += remains;
			if (num >= 10)
			{
				remains = num / 10;
			}
			else
				remains = 0;
			num %= 10;
			new_digits[i] = (char)(num + '0');
		}
		if (remains)
			new_digits[i++] = (char)(remains + '0');

		while (--i)
		{
			if (new_digits[i] != '0')
				break;
		}
		i++;
		//
		//
		//		unsigned int remains = 0;
		//		size_t i = 0;
		//		for (; i < n_one + n_two - 3; i++)
		//		{
		//			unsigned int num = 0;
		//
		//			for (size_t j = ((i + 1) > (n_one - 1)) ? (i + 1) - (n_one - 1) : 0; j < MIN(n_two - 1, i + 1); j++)
		//			{
		//				num += (one[i - j] - '0') * (two[j] - '0');
		//			}
		//			num += remains;
		//			if (num >= 10)
		//			{
		//				remains = num / 10;
		//			}
		//			else
		//				remains = 0;
		//			num %= 10;
		//			new_digits[i] = (char)(num + '0');
		//		}
		//		if (remains)
		//			new_digits[i++] = (char)(remains + '0');

		new_digits[i++] = '\0';

		if (n_one + n_two != i)
		{
			char *new_new_digits = (char *)realloc(new_digits, (i) * sizeof(char));
			if (!new_new_digits)
				throw ERROR_OUTOFMEMORY;

			new_digits = new_new_digits;
		}

		// printf("simple - %zu, \"%s\"\n", i,  new_digits);

		return { new_digits, i };
	}

	size_t len = MAX(n_one, n_two);
	// printf("len = \"%zu\"\n", len);

	Result_multiply_Karatsuba last = multiply_Karatsuba(one, MIN((len + 1) / 2, n_one), two, MIN((len + 1) / 2, n_two));

	// printf("last - %zu, \"%s\"\n", last.length, last.digits);
	Result_multiply_Karatsuba first;
	if (n_one <= (len + 1) / 2 || n_two <= (len + 1) / 2)
	{
		first.digits = (char *)malloc((2) * sizeof(char));
		if (!first.digits)
			throw ERROR_OUTOFMEMORY;
		first.digits[0] = '0';
		first.digits[1] = '\0';
		first.length = 2;
	}
	else
	{
		first = multiply_Karatsuba(one + (len + 1) / 2, n_one - (len + 1) / 2, two + (len + 1) / 2, n_two - (len + 1) / 2);
	}

	// printf("first - %zu, \"%s\"\n", first.length, first.digits);

	LN one_first(one, MIN((len + 1) / 2, n_one), true);
	// printf("one_first before - %zu, \"%s\"\n", one_first.n, one_first.digits);
	if (n_one > (len + 1) / 2)
	{
		LN one_last(one + (len + 1) / 2, n_one - (len + 1) / 2, true);
		// printf("one_last before - %zu, \"%s\"\n", one_last.n, one_last.digits);
		one_first += one_last;
	}

	// printf("one_first - %zu, \"%s\"\n", one_first.n, one_first.digits);

	LN two_first(two, MIN((len + 1) / 2, n_two), true);
	// printf("two_first before - %zu, \"%s\"\n", two_first.n, two_first.digits);
	if (n_two > (len + 1) / 2)
	{
		LN two_last(two + (len + 1) / 2, n_two - (len + 1) / 2, true);
		// printf("two_last before - %zu, \"%s\"\n", two_last.n, two_last.digits);
		two_first += two_last;
		// printf("two_first - %zu, \"%s\"\n", two_first.n, two_first.digits);
	}

	// printf("two_first - %zu, \"%s\"\n", two_first.n, two_first.digits);

	Result_multiply_Karatsuba second = multiply_Karatsuba(one_first.digits, one_first.n - 1, two_first.digits, two_first.n - 1);

	// printf("second - %zu, \"%s\"\n", second.length, second.digits);

	LN ln_first(first.digits, first.length);
	LN ln_second(second.digits, second.length);
	LN ln_last(last.digits, last.length);

	ln_second -= (ln_first + ln_last);

	// printf("real second - %zu, \"%s\"\n", ln_second.n, ln_second.digits);

	size_t len_new_digits = MAX(ln_first.n + len + len % 2, ln_second.n + (len + 1) / 2);
	char *new_digits = (char *)malloc((len_new_digits) * sizeof(char));
	if (!new_digits)
		throw ERROR_OUTOFMEMORY;

	for (size_t i = 0; i < len_new_digits; i++)
		new_digits[i] = 0;
	new_digits[len_new_digits - 1] = '\0';

	// printf("before new_digits - %zu, \"%s\"\n", MAX(ln_first.n + len + len%2, ln_second.n + (len + 1)/2),
	// new_digits);

	for (size_t i = 0; i < ln_last.n - 1; i++)
	{
		new_digits[i] = ln_last.digits[i] - '0';
		// printf("%zu) %d %d\n", i, ln_last.digits[i] - '0', new_digits[i]);
	}
	// printf("\n");

	for (size_t i = (len + 1) / 2; i < ln_second.n - 1 + (len + 1) / 2; i++)
	{
		new_digits[i] += ln_second.digits[i - (len + 1) / 2] - '0';
		// printf("%zu)) %d %d\n", i, ln_second.digits[i - (len+1)/2] - '0', new_digits[i]);
	}
	// printf("\n");

	for (size_t i = len + len % 2; i < ln_first.n - 1 + len + len % 2; i++)
	{
		new_digits[i] += ln_first.digits[i - (len + len % 2)] - '0';
		// printf("%zu))) %d %d\n", i,  ln_first.digits[i - (len + len%2)] - '0', new_digits[i]);
	}
	// printf("\n");

	unsigned int remains = 0;
	size_t i = 0;
	for (; i < len_new_digits - 1; i++)
	{
		// printf("%d - ", new_digits[i]);
		new_digits[i] += remains;
		if (new_digits[i] > 9)
		{
			remains = new_digits[i] / 10;
			new_digits[i] %= 10;
		}
		else
		{
			remains = 0;
		}

		new_digits[i] += '0';
		// printf("%c %d\n", new_digits[i], new_digits[i]);
	}
	// printf("\n");
	if (remains)
	{
		new_digits[i++] = remains + '0';
	}
	while (--i)
	{
		if (new_digits[i] != '0')
			break;
	}
	i++;

	if (i != len_new_digits - 1)
	{
		char *new_new_digits = (char *)realloc(new_digits, (i + 1) * sizeof(char));
		if (!new_new_digits)
			throw ERROR_OUTOFMEMORY;

		new_digits = new_new_digits;
		new_digits[i] = '\0';
		// printf("rem OK - %zu, \"%s\"\n", i + 1, new_digits);
		return { new_digits, i + 1 };
	}
	else
	{
		// printf("OK - %zu, \"%s\"\n", len_new_digits, new_digits);
		return { new_digits, len_new_digits };
	}
}

void LN::multiply(const LN &multiplier)
{
	Result_multiply_Karatsuba res = multiply_Karatsuba(digits, n - 1, multiplier.digits, multiplier.n - 1);
	free(digits);
	digits = res.digits;
	n = res.length;

	//	char *new_digits = (char *)malloc((multiplier.n + n) * sizeof(char));
	//	if (!new_digits)
	//		throw ERROR_OUTOFMEMORY;
	//
	//	unsigned int remains = 0;
	//	size_t i = 0;
	//	for (; i < multiplier.n + n - 3; i++)
	//	{
	//		unsigned int num = 0;
	//
	//		for (size_t j = ((i + 1) > (n - 1)) ? (i + 1) - (n - 1) : 0; j < MIN(multiplier.n - 1, i + 1); j++)
	//		{
	//			num += (digits[i - j] - '0') * (multiplier.digits[j] - '0');
	//		}
	//		num += remains;
	//		if (num >= 10)
	//		{
	//			remains = num / 10;
	//		}
	//		else
	//			remains = 0;
	//		num %= 10;
	//		new_digits[i] = (char)(num + '0');
	//	}
	//	if (remains)
	//		new_digits[i++] = (char)(remains + '0');
	//
	//	free(digits);
	//	digits = new_digits;
	//
	//	end_number(i);
}

void LN::divide(const LN &divider)	  /// Newton 's method
{
	LN positive_divider(divider);
	if (positive_divider.isNegative)
		positive_divider.isNegative = false;

	bool my_isNegative = isNegative;
	isNegative = false;

	LN result((long long)((1.0 / (positive_divider.digits[positive_divider.n - 2] - '0')) * 10));
	if (result.n == 3)
	{
		result.digits[0] = '1';
		result.end_number(1);
	}

	size_t required_number_digits = n - positive_divider.n + 5;
	LN two(0ll);

	LN *prev = new LN((*this) * result);	// (*this) * positive_divider

	while (true)
	{
		size_t two_n = result.n - 1 + positive_divider.n - 1;
		two.end_number(two_n);
		two.digits[--two_n] = '2';
		while (two_n--)
			two.digits[two_n] = '0';

		two -= LN(result * positive_divider);

		result *= two;

		if (result.n > required_number_digits)
		{
			for (size_t i = 0, shift = result.n - required_number_digits; i < required_number_digits; i++)
			{
				result.digits[i] = result.digits[i + shift];
			}
			result.reallocate(required_number_digits);
		}

		LN *next = new LN((*this) * result);

		if (next->n >= required_number_digits)
		{
			bool eq = true;
			for (size_t i = 1; i <= required_number_digits; i++)
			{
				if (prev->digits[prev->n - 1 - i] != next->digits[next->n - 1 - i])
				{
					eq = false;
					break;
				}
			}
			if (eq)
			{
				delete prev;
				prev = next;
				break;
			}
		}
		delete prev;
		prev = next;
	}

	prev->leave_end(prev->n - result.n + 2);	// - result.n - positive_divider.n + 3);

	(*prev) += LN(1ll);
	prev->leave_end(prev->n - positive_divider.n + 1);

	(*prev) += 1_ln;

	if ((*prev) * positive_divider > (*this))
		(*prev) -= 1_ln;

	*this = *prev;
	delete prev;

	isNegative = my_isNegative;
}

void LN::division_by_two()
{
	digits[0] = (char)(((digits[0] - '0') / 2) + '0');
	for (size_t i = 1; i < n - 1; i++)
	{
		if ((digits[i] - '0') % 2 != 0)
		{
			digits[i - 1] = (char)(digits[i - 1] + 5);
			digits[i] = (char)(((digits[i] - '0') / 2) + '0');
		}
		else
			digits[i] = (char)(((digits[i] - '0') / 2) + '0');
	}
	if (digits[n - 2] == '0')
		end_number(n - 2);
}

void LN::taking_square_root()	 /// Newton 's method
{
	if (n == 2)
	{
		if (digits[0] < '4')
			digits[0] = '1';
		else if (digits[0] >= '4' && digits[0] < '9')
			digits[0] = '2';
		else
			digits[0] = '3';
		return;
	}

	LN result(0ll);

	if ((n - 1) % 2 == 0)
	{
		switch (digits[n - 2])
		{
		case '1':
			result = 3_ln;
			break;
		case '2':
			result = 2_ln;
			break;
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			result = 1_ln;
			break;
		}
	}
	else
	{
		switch (digits[n - 2])
		{
		case '1':
		{
			size_t i = 0;
			for (; i < n - 2; i++)
				if (digits[i] != '0')
					break;

			if (i == n - 2)
			{
				end_number(n / 2);
				digits[n - 2] = '1';
				return;
			}
			else
				result = 9_ln;

			break;
		}
		case '2':
			result = 7_ln;
			break;
		case '3':
			result = 6_ln;
			break;
		case '4':
			result = 5_ln;
			break;
		case '5':
		case '6':
		case '7':
		case '8':
			result = 4_ln;
			break;
		case '9':
			result = 3_ln;
			break;
		}
	}

	size_t required_number_digits = n / 2 + 1 + 2;
	LN three(0ll);

	LN *prev = new LN((*this) * result);

	while (true)
	{
		LN deductible(result * result * (*this));

		size_t two_n = deductible.n - 1;
		if (deductible.digits[deductible.n - 2] > '2')
			two_n++;
		three.end_number(two_n);
		three.digits[--two_n] = '3';
		while (two_n--)
			three.digits[two_n] = '0';

		three -= deductible;
		result *= three;
		result.division_by_two();

		if (result.n > required_number_digits)
		{
			for (size_t i = 0, shift = result.n - required_number_digits; i < required_number_digits; i++)
			{
				result.digits[i] = result.digits[i + shift];
			}
			result.reallocate(required_number_digits);
		}

		LN *next = new LN((*this) * result);
		if (next->n >= required_number_digits)
		{
			bool eq = true;
			for (size_t i = 1; i <= required_number_digits; i++)
			{
				if (prev->digits[prev->n - 1 - i] != next->digits[next->n - 1 - i])
				{
					eq = false;
					break;
				}
			}
			if (eq)
			{
				delete prev;
				prev = next;
				break;
			}
		}
		delete prev;
		prev = next;
	}

	prev->leave_end(required_number_digits - 2);
	(*prev) += 1_ln;

	if ((*prev) * (*prev) > (*this))
		(*prev) -= 1_ln;

	*this = *prev;
	delete prev;
}

void LN::sqrt()
{
	if (isNaN)
	{
		return;
	}
	if (isNegative)
	{
		make_nan();
		return;
	}
	if (!(*this))
	{
		return;
	}
	if (n == 2 && digits[0] == '1')
	{
		return;
	}

	taking_square_root();
}

LN::Result_Comparison LN::comparison(const LN &second) const
{
	if (&second == this)
		return equally;

	if (n < second.n)
		return less;
	else if (n > second.n)
		return more;
	else
	{
		for (size_t i = n - 1; i--;)
		{
			if (digits[i] < second.digits[i])
				return less;
			else if (digits[i] > second.digits[i])
				return more;
		}
		return equally;
	}
}

LN::Result_Comparison LN::full_comparison(const LN &second) const
{
	Result_Comparison res;
	if (isNegative == second.isNegative)
	{
		res = comparison(second);

		if (isNegative)
		{
			switch (res)
			{
			case less:
				res = more;
				break;
			case more:
				res = less;
				break;
			case equally:
				break;
			}
		}
	}
	else
	{
		if (isNegative)
			res = less;
		else
			res = more;
	}

	return res;
}

void LN::make_zero()
{
	digits[0] = '0';
	end_number(1);
	isNegative = false;
}

void LN::make_nan()
{
	isNaN = true;
	end_number(3);
	digits[0] = 'N';
	digits[1] = 'a';
	digits[2] = 'N';
	isNegative = false;
}

void LN::subtractive_selection(const LN &deductible)
{
	switch (comparison(deductible))
	{
	case less:
	{
		minus(deductible, false);
		isNegative = !isNegative;
		break;
	}
	case more:
	{
		minus(deductible, true);
		break;
	}
	case equally:
	{
		make_zero();
		break;
	}
	}
}

void LN::operation_handler(const LN &second, void (LN::*op)(const LN &))
{
	if (isNaN)
		return;
	if (second.isNaN)
	{
		make_nan();
		return;
	}
	(this->*op)(second);
}

void LN::addition(const LN &summand)
{
	if (isNegative == summand.isNegative)
		plus(summand);
	else
		subtractive_selection(summand);
}

void LN::subtraction(const LN &deductible)
{
	if (isNegative == deductible.isNegative)
		subtractive_selection(deductible);
	else
		plus(deductible);
}

void LN::multiplication(const LN &multiplier)
{
	if (!(*this))
	{
		return;
	}
	if (!(multiplier))
	{
		make_zero();
		return;
	}

	if (!(multiplier.n == 2 && multiplier.digits[0] == '1'))
		multiply(multiplier);

	isNegative = (isNegative != multiplier.isNegative);
}

void LN::division(const LN &divider)
{
	if (!divider)
	{
		make_nan();
		return;
	}

	if (divider.n == 2 && divider.digits[0] == '1')
	{
		isNegative = (isNegative != divider.isNegative);
		return;
	}

	switch (comparison(divider))
	{
	case more:
		divide(divider);
		isNegative = (isNegative != divider.isNegative);
		return;
	case less:
	{
		make_zero();
		return;
	}
	case equally:
	{
		digits[0] = '1';
		end_number(1);
		isNegative = (isNegative != divider.isNegative);
		return;
	}
	}
}

void LN::remainder_of_division(const LN &divider)
{
	(*this) -= ((*this) / divider) * divider;
}

LN operator+(const LN &first, const LN &second)
{
	return LN(first) += second;
}
LN operator-(const LN &first, const LN &second)
{
	return LN(first) -= second;
}
LN operator*(const LN &first, const LN &second)
{
	return LN(first) *= second;
}
LN operator/(const LN &first, const LN &second)
{
	return LN(first) /= second;
}
LN operator%(const LN &first, const LN &second)
{
	return LN(first) %= second;
}

LN &LN::operator=(const LN &copied)
{
	if (&copied == this)
		return *this;

	free(digits);

	n = copied.n;
	digits = (char *)malloc(n * sizeof(char));
	if (!digits)
		throw ERROR_OUTOFMEMORY;

	isNegative = copied.isNegative;
	isNaN = copied.isNaN;

	for (size_t i = 0; i < n; i++)
		digits[i] = copied.digits[i];

	return *this;
}

LN &LN::operator=(LN &&movable) noexcept
{
	if (&movable == this)
		return *this;

	free(digits);
	n = movable.n;
	digits = movable.digits;
	isNegative = movable.isNegative;
	isNaN = movable.isNaN;

	movable.digits = nullptr;

	return *this;
}

void LN::leave_end(size_t k)
{
	for (size_t i = 0, shift = n - k; i < k; i++)
	{
		digits[i] = digits[i + shift];
	}
	reallocate(k);
}