#include "LN.h"
#include "return_codes.h"
#include <forward_list>
#include <string_view>

#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

///--------------------------------NOT AN ACTIVE PROJECT FILE--------------------------------///

void test()
{
#ifdef TEST
#	if TEST == 0
	//	LN first(std::string_view("3245532849598643589698634")); //2132486941
	//	LN second("7");
	//
	//	//first /= second;
	//
	//	//long_long_dvide((long long)first, (long long)second);
	//	//std::cout <<    "END LONG LONG\n\n" << std::endl;
	//	ln_dvide(first, second);

	LN result((long long)1000000);	  // 2132486941

	result.sqrt();

	printf("result = \"");
	result.print();
	printf("\"\n");

#	elif TEST == 1

	for (int i = 0; i <= 1000; i++)
	{
		if (i % 100 == 0)
			printf("test %d\n", i);

		for (int j = -1000; j <= 1000; j++)
		{
			{
				LN first((long long)i);
				LN second((long long)j);
				first += second;

				long long ln_ans = (long long)first;
				long long answer = i + j;
				if (ln_ans != answer)
				{
					printf("ERROR i=%d j=%d\n", i, j);
					printf("ln_ans: \"%lld\"\n", ln_ans);
					printf("answer: \"%lld\"\n", answer);
					return -1;
				}

				//				char ln_ans[50];
				//				first.print_str(ln_ans);
				//				char answer[50];
				//				sprintf(answer, "%d", i + j);
				//				if(strcmp(ln_ans, answer))
				//				{
				//					printf("ERROR i=%d j=%d\n", i, j);
				//					printf("ln_ans: \"%s\"\n", ln_ans);
				//					printf("answer: \"%s\"\n", answer);
				//					return -1;
				//				}
			}

			{
				LN first((long long)i);
				LN second((long long)j);

				first -= second;
				long long ln_ans = (long long)first;
				long long answer = i - j;
				if (ln_ans != answer)
				{
					printf("ERROR i=%d j=%d\n", i, j);
					printf("ln_ans: \"%lld\"\n", ln_ans);
					printf("answer: \"%lld\"\n", answer);
					return -1;
				}
			}

			{
				LN first((long long)i);
				LN second((long long)j);

				first *= second;
				long long ln_ans = (long long)first;
				long long answer = i * j;
				if (ln_ans != answer)
				{
					printf("ERROR i=%d j=%d\n", i, j);
					printf("ln_ans: \"%lld\"\n", ln_ans);
					printf("answer: \"%lld\"\n", answer);
					return -1;
				}
			}

			{
				if (j != 0)
				{
					LN first((long long)i);
					LN second((long long)j);

					first /= second;
					long long ln_ans = (long long)first;
					long long answer = i / j;
					if (ln_ans != answer)
					{
						printf("ERROR i=%d j=%d\n", i, j);
						printf("ln_ans: \"%lld\"\n", ln_ans);
						printf("answer: \"%lld\"\n", answer);
						return -1;
					}
				}
			}
		}

		{
			if (i >= 0 && i % 10 != 0)
			{
				LN first((long long)i);

				first.sqrt();
				long long ln_ans = (long long)first;
				long long answer = sqrt(i);
				if (ln_ans != answer)
				{
					printf("ERROR i=%d\n", i);
					printf("ln_ans: \"%lld\"\n", ln_ans);
					printf("answer: \"%lld\"\n", answer);
					return -1;
				}
			}
		}
	}
	printf("OK\n");

#	endif
#endif
}

///--------------------------------NOT AN ACTIVE PROJECT FILE--------------------------------///