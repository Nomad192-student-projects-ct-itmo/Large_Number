#ifndef MY_LOGGING_H
#define MY_LOGGING_H

//#define LOG_ON verbose

#define LOGS_NAME "LN_logs.txt"
#define CLEAR_FILE_LOG

#include "return_codes.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>

typedef enum type_logging_t
{
	critical_error,
	error,
	warning,
	info,
	verbose
} Type_Logging [[maybe_unused]];

/// Does not output an error when not clearing the file
void clear_log_file();

int my_log(bool w_time, Type_Logging level, size_t line, const char *str_log, ...);

int vmy_log(bool w_time, Type_Logging level, size_t line, const char *str_log, va_list args);

int my_print_err(Type_Logging level, size_t line, const char *str_err, ...);

#endif	  // MY_LOGGING_H