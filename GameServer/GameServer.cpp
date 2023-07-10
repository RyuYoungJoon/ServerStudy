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

class TestLock
{
	USE_LOCK;
public:
	int32 TestRead()
	{
		READ_LOCK;
		if (_queue.empty())
			return -1;

		return _queue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;

		_queue.push(rand() % 100);
	}

	void TestPop()
	{
		WRITE_LOCK;
		if (_queue.empty() == false)
			_queue.pop();
	}

private:
	queue<int32> _queue;

};

TestLock testLock;

void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}

}

void ThreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}

}

int main()
{
	for (uint32 i = 0; i < 2; ++i)
	{
		GThreadManger->Launch(ThreadWrite);
	}

	for (uint32 i = 0; i < 5; ++i)
	{
		GThreadManger->Launch(ThreadRead);
	}

	GThreadManger->Join();
}
