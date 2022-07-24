#include "my_logging.h"

void clear_log_file()
{
#ifdef CLEAR_FILE_LOG
	remove(LOGS_NAME);
#endif
}

int vmy_log(const bool w_time, const Type_Logging level, const size_t line, const char *str_log, va_list args)
{
#ifdef LOG_ON
	if (LOG_ON >= level)
	{
		FILE *out_logs = fopen(LOGS_NAME, "a");
		if (!out_logs)
		{
			printf("I can't open a file for logging errors");
			return ERROR_NOT_FOUND;
		}
		if (w_time)
		{
			time_t current_time;
			current_time = time(nullptr);
			fprintf(out_logs, "%s ", ctime(&current_time));
		}
		if (line != 0)
			fprintf(out_logs, "in line - %zu\n", line);
		vfprintf(out_logs, str_log, args);

		fclose(out_logs);
	}
#endif
	return 0;
}

int my_log(const bool w_time, const Type_Logging level, const size_t line, const char *str_log, ...)
{
	int result = 0;
#ifdef LOG_ON
	va_list args;
	va_start(args, str_log);
	result = vmy_log(w_time, level, line, str_log, args);
	va_end(args);
#endif
	return result;
}

int my_print_err(const Type_Logging level, const size_t line, const char *str_err, ...)
{
	int result = 0;
#ifdef LOG_ON
	va_list args;
	va_start(args, str_err);
	vfprintf(stderr, str_err, args);
	result = vmy_log(true, level, line, str_err, args);
	va_end(args);
#endif
	return result;
}