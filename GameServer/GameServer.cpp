#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <windows.h>
#include <future>

// _declspec(thread) int32 value;
// TLS : 쓰레드마다 자신만 갖고있는 공유 메모리
thread_local int32 LThreadId = 0;


void ThreadMain(int32 threadid)
{
	LThreadId = threadid;
	while (true)
	{
		cout << " HI! I am Thread " << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;
	
	for (int32 i = 0; i < 10; ++i) {
		int32 threadId = i + 1;
		threads.push_back(thread(ThreadMain, threadId));
	}
	for (thread& t : threads)
		t.join();
}
