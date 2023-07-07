#pragma once

/*--------------------
		Crash
---------------------*/

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	_Analysis_assume_(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

// 조건부 크래쉬 expr 값에 따라
#define ASSERT_CRASH(expr)			\
{									\
	if(!(expr))						\
	{								\
		CRASH("ASSERT_CRASH");		\
		_Analysis_assume_(expr);	\
	}								\
}