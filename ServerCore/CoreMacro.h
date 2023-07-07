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

// ���Ǻ� ũ���� expr ���� ����
#define ASSERT_CRASH(expr)			\
{									\
	if(!(expr))						\
	{								\
		CRASH("ASSERT_CRASH");		\
		_Analysis_assume_(expr);	\
	}								\
}