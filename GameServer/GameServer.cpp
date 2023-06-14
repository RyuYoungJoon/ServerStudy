#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <windows.h>
#include <future>
#include "CuncurentQueue.h"
#include "ConcurentStack.h"

LockQueue<int32> q;
LockFreeStack<int32> s;


void Push()
{
	while (true)
	{
		int32 value = rand() % 100;
		s.Push(value);

		this_thread::sleep_for(10ms);
	}
}

void Pop()
{
	while (true)
	{

		int32 data = 0;
		if(s.TryPop(OUT data))
			cout << data << endl;
	}


}
int main()
{
	thread t1{ Push };
	thread t2{ Push };
	thread t3{ Pop };
	thread t4{ Pop };

	t1.join();
	t2.join();
	t3.join();
	t4.join();
}
