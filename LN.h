#ifndef LN_H
#define LN_H

//#define TEST 1

#include "return_codes.h"
#include <string_view>

#include <cstdio>
#include <cstdlib>

#define START_N_DIGITS (20)

class LN
{
  private:
	size_t n = START_N_DIGITS;
	char *digits = (char *)malloc(START_N_DIGITS * sizeof(char));
	bool isNegative;
	bool isNaN = false;
	LN(const char *digits, size_t n);
	LN(const char *new_digits, size_t new_n, bool isCopy);

  public:
	explicit LN(bool isNegative) : isNegative(isNegative)
	{
		if (!digits)
			throw ERROR_OUTOFMEMORY;
	}
	explicit LN(long long number = 0);
	explicit LN(const char *number);

	explicit LN(std::string_view number);

	LN(const LN &copied);
	LN(LN &&movable) noexcept;

	LN &operator=(const LN &copied);
	LN &operator=(LN &&movable) noexcept;

	explicit operator bool() const { return !(n == 2 && digits[0] == '0'); }
	explicit operator long long() const;
	~LN() noexcept { free(digits); }

	bool scan(FILE *in);
	void print_file(FILE *out) const noexcept;
	[[maybe_unused]] void print_str(char *out) const noexcept;
	[[maybe_unused]] void print() const noexcept;
	[[nodiscard]] char *get_str() const noexcept;
	void invert()
	{
		if (*this && !isNaN)
			isNegative = !isNegative;
	}
	void make_nan();

  private:
	[[nodiscard]] bool comparison_handler(const LN &second) const { return !(isNaN || second.isNaN); }
	void division_by_two();
	void make_zero();
	void operation_handler(const LN &second, void (LN::*op)(const LN &));
	void leave_end(size_t k);
	void reallocate();
	void reallocate(size_t size);
	void add_digit(char d, size_t ind);
	void end_number(size_t size);
	void revers();
	void plus(const LN &summand);
	void minus(const LN &second, bool is_second_deductible);
	void multiply(const LN &multiplier);
	void divide(const LN &multiplier);
	void taking_square_root();
	struct Result_multiply_Karatsuba
	{
		char *digits;
		size_t length;
	};
	static Result_multiply_Karatsuba multiply_Karatsuba(const char *one, size_t n_one, const char *two, size_t n_two);
	enum Result_Comparison
	{
		less = -1,
		equally = 0,
		more = 1
	};

	[[nodiscard]] Result_Comparison comparison(const LN &second) const;
	[[nodiscard]] Result_Comparison full_comparison(const LN &second) const;

	void subtractive_selection(const LN &deductible);
	void addition(const LN &summand);
	void subtraction(const LN &deductible);
	void multiplication(const LN &multiplier);
	void division(const LN &multiplier);
	void remainder_of_division(const LN &divider);

  public:
	void sqrt();
	LN &operator+=(const LN &summand)
	{
		operation_handler(summand, &LN::addition);
		return *this;
	}
	LN &operator-=(const LN &deductible)
	{
		operation_handler(deductible, &LN::subtraction);
		return *this;
	}
	LN &operator*=(const LN &multiplier)
	{
		operation_handler(multiplier, &LN::multiplication);
		return *this;
	}
	LN &operator/=(const LN &divider)
	{
		operation_handler(divider, &LN::division);
		return *this;
	}
	LN &operator%=(const LN &divider)
	{
		operation_handler(divider, &LN::remainder_of_division);
		return *this;
	}
	bool operator==(const LN &second) { return comparison_handler(second) && full_comparison(second) == equally; }
	bool operator<(const LN &second) { return comparison_handler(second) && full_comparison(second) == less; }
	bool operator<=(const LN &second) { return comparison_handler(second) && full_comparison(second) != more; }
	bool operator>(const LN &second) { return comparison_handler(second) && full_comparison(second) == more; }
	bool operator>=(const LN &second) { return comparison_handler(second) && full_comparison(second) != less; }
	bool operator!=(const LN &second) { return !comparison_handler(second) || full_comparison(second) != equally; }
};

LN operator+(const LN &first, const LN &second);
LN operator-(const LN &first, const LN &second);
LN operator*(const LN &first, const LN &second);
LN operator/(const LN &first, const LN &second);
LN operator%(const LN &first, const LN &second);

LN operator"" _ln(const char *str);

#endif	  // LN_H