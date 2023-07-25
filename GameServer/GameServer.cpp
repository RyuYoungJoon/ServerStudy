#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"
#include "Allocator.h"

class Player
{
public:
	Player(){}
	virtual ~Player() {}
};
class Knight : public Player
{
public:
	Knight()
	{
		cout << "생성자" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "소멸자" << endl;
	}

	int32 _hp = 100;
	int32 _mp = 10;
};

int main()
{
	// 왜 메모리 풀링?
	// 해제 -> 할당 / 해제 -> 할당 반복
	// 컨텍스트 스위칭 자주 발생
	// [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ] 메모리 사용 못하는 영역 발생 가능

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManger->Launch([]()
			{
				while (true)
				{
					Vector<Knight> v(10);

					Map<int32, Knight> m;
					m[100] = Knight();

					this_thread::sleep_for(100ms);
				}
			});
	}

	GThreadManger->Join();
}
