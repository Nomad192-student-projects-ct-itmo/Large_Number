#include "my_ptr_unchained.h"

void my_fclose(FILE **file)
{
	if (*file != nullptr)
	{
		fclose(*file);
		*file = nullptr;
	}
}