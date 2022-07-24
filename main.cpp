#include "LN.h"
#include "my_logging.h"
#include "my_ptr_unchained.h"
#include "return_codes.h"
#include <forward_list>

#include <cstdio>

int main(int argc, char **argv)
{
	clear_log_file();
	my_log(false, verbose, 0, "\nNEW PROGRAM\n");
	my_log(true, verbose, 0, "Program \"%s\" starts with arguments: ", argv[0]);
	for (int i = 1; i < argc; i++)
	{
		my_log(false, verbose, 0, "%s ", argv[i]);
	}
	my_log(false, verbose, 0, "\n\n");

	if (argc != 3)
	{
		fprintf(
			stderr,
			"HELP: "
			"\t first argument ->  name_in_file "
			"(with file extension REQUIRED and path if necessary)"
			"\n\tsecond argument -> name_out_file "
			"(with file extension REQUIRED and path if necessary)\n\n");
		my_print_err(
			critical_error,
			__LINE__,
			"I was given the wrong "
			"number of arguments: %d instead of 2\nEND\n\n",
			argc - 1);
		for (int i = 1; i < argc; i++)
		{
			my_print_err(critical_error, __LINE__, "arg %d = \"%s\"\n", i, argv[i]);
		}
		return ERROR_INVALID_PARAMETER;
	}

	///-------------------------------------------------------------------------------------------------------------///

	FILE *in = nullptr;
	FILE *out = nullptr;

	std::forward_list< LN > numbers_stack;

	int return_code = ERROR_SUCCESS;	// the value that the program will return

	///-------------------------------------------------------------------------------------------------------------///

	in = fopen(argv[1], "r");
	if (in == nullptr)
	{
		my_print_err(critical_error, __LINE__, "I can't open the input file - \"%s\"\n\n", argv[1]);
		return_code = ERROR_FILE_NOT_FOUND;
		goto close;
	}

	try
	{
		char c;
		while (fscanf(in, "%c", &c) != EOF)
		{
			if (c >= '0' && c <= '9')
			{
				ungetc(c, in);
				numbers_stack.emplace_front(false);
				if (!numbers_stack.front().scan(in))
				{
					my_print_err(critical_error, __LINE__, "I can't read number\n\n");
					return_code = ERROR_INVALID_DATA;
					goto close;
				}
				my_log(false, info, __LINE__, "Read number \"%s\"\n\n", numbers_stack.front().get_str());
			}
			else
			{
				switch (c)
				{
				case '!':
				{
					if (fscanf(in, "%c", &c) != EOF && c == '=')
					{
						bool res = ((*(++numbers_stack.begin())) != numbers_stack.front());
						numbers_stack.pop_front();
						numbers_stack.pop_front();
						numbers_stack.emplace_front((long long)res);
					}
					else
					{
						my_print_err(critical_error, __LINE__, "Incorrect input - \"!%c\"\n\n", c);
						return_code = ERROR_INVALID_DATA;
						goto close;
					}
					break;
				}
				case '=':
				{
					if (fscanf(in, "%c", &c) != EOF && c == '=')
					{
						bool res = ((*(++numbers_stack.begin())) == numbers_stack.front());
						numbers_stack.pop_front();
						numbers_stack.pop_front();
						numbers_stack.emplace_front((long long)res);
					}
					else
					{
						my_print_err(critical_error, __LINE__, "Incorrect input - \"=%c\"\n\n", c);
						return_code = ERROR_INVALID_DATA;
						goto close;
					}
					break;
				}
				case '<':
				{
					if (fscanf(in, "%c", &c) != EOF)
					{
						if (c == '=')
						{
							bool res = ((*(++numbers_stack.begin())) <= numbers_stack.front());
							numbers_stack.pop_front();
							numbers_stack.pop_front();
							numbers_stack.emplace_front((long long)res);
							break;
						}
						else
						{
							ungetc(c, in);
						}
					}

					bool res = ((*(++numbers_stack.begin())) < numbers_stack.front());
					numbers_stack.pop_front();
					numbers_stack.pop_front();
					numbers_stack.emplace_front((long long)res);
					break;
				}
				case '>':
				{
					if (fscanf(in, "%c", &c) != EOF)
					{
						if (c == '=')
						{
							bool res = ((*(++numbers_stack.begin())) >= numbers_stack.front());
							numbers_stack.pop_front();
							numbers_stack.pop_front();
							numbers_stack.emplace_front((long long)res);
							break;
						}
						else
						{
							ungetc(c, in);
						}
					}

					bool res = ((*(++numbers_stack.begin())) > numbers_stack.front());
					numbers_stack.pop_front();
					numbers_stack.pop_front();
					numbers_stack.emplace_front((long long)res);
					break;
				}
				case '+':
				{
					(*(++numbers_stack.begin())) += numbers_stack.front();
					numbers_stack.pop_front();
					break;
				}
				case '*':
				{
					(*(++numbers_stack.begin())) *= numbers_stack.front();
					numbers_stack.pop_front();
					break;
				}
				case '/':
				{
					(*(++numbers_stack.begin())) /= numbers_stack.front();
					numbers_stack.pop_front();
					break;
				}
				case '%':
				{
					(*(++numbers_stack.begin())) %= numbers_stack.front();
					numbers_stack.pop_front();
					break;
				}
				case '-':
				{
					if (fscanf(in, "%c", &c) != EOF && !(c == '\n' || c == ' ' || c == '\t'))
					{
						ungetc(c, in);
						numbers_stack.emplace_front(true);
						if (!numbers_stack.front().scan(in))
						{
							my_print_err(critical_error, __LINE__, "I can't read number\n\n");
							return_code = ERROR_INVALID_DATA;
							goto close;
						}
						my_log(false, info, __LINE__, "Read negative number \"%s\"\n\n", numbers_stack.front().get_str());
					}
					else
					{
						(*(++numbers_stack.begin())) -= numbers_stack.front();
						numbers_stack.pop_front();
					}

					break;
				}
				case '_':
				{
					numbers_stack.front().invert();
					break;
				}
				case '~':
				{
					numbers_stack.front().sqrt();
					break;
				}
				case 'N':
				{
					fscanf(in, "%c", &c);
					if (c == 'a')
					{
						fscanf(in, "%c", &c);
						if (c == 'N')
						{
							numbers_stack.emplace_front(false);
							numbers_stack.front().make_nan();
							break;
						}
					}
				}
				default:
				{
					my_print_err(critical_error, __LINE__, "NOT_IMPLEMENTED\n\n", c);
					return_code = ERROR_NOT_IMPLEMENTED;
					goto close;
				}
				}
			}
			fscanf(in, "\n");
		}
	} catch (int e)
	{
		my_print_err(critical_error, __LINE__, "LN exception - %d\n\n", e);
		return_code = e;
		goto close;
	} catch (...)
	{
		my_print_err(critical_error, __LINE__, "Unknown exception\n\n");
		return_code = ERROR_UNKNOWN;
		goto close;
	}

	my_fclose(&in);	   // to check and to return NULL to the released file
	my_log(true, verbose, __LINE__, "Data from the %s (input file) was successfully read\n\n", argv[1]);

	///-------------------------------------------------------------------------------------------------------------///

	out = fopen(argv[2], "w");
	if (out == nullptr)
	{
		my_print_err(critical_error, __LINE__, "I can't create the output file - \"%s\"\n\n", argv[2]);
		return_code = ERROR_FILE_NOT_FOUND;
		goto close;
	}

	while (!numbers_stack.empty())
	{
		numbers_stack.front().print_file(out);
		fprintf(out, "\n");
		numbers_stack.pop_front();
	}

	my_fclose(&out);
	my_log(true, verbose, __LINE__, "The response was successfully written to the %s (output file)\n\n", argv[2]);

	///-------------------------------------------------------------------------------------------------------------///

close:
	my_fclose(&in);
	my_fclose(&out);

	my_log(true, verbose, 0, "The program ended successfully\nEND\n\n");

	return return_code;
}