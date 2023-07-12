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

// 소수 구하기
bool IsPrime(int number)
{
	if (number <= 1)
		return false;
	if (number == 2 || number == 3)
		return true;

	for (int i = 2; i < number; ++i)
	{
		if ((number % i) == 0)
			return false;
	}

	return true;

}

// [start - end]
int CountPrime(int start, int end)
{
	int count = 0;

	for (int number = start; number <= end; number++)
	{
		if (IsPrime(number))
			count++;
	}
	return count;
}
// 1과 자기 자신으로만 나뉘면 그것을 소수

int main()
{
	const int MAX_NUMBER = 100'0000;
	//int cnt = 0;
	//for (int i = 2; i <= MAX_NUMBER; ++i)
	//{
	//	cnt = 0;
	//	for (int j = 2; j <= i; ++j)
	//	{
	//		if (i % j == 0)
	//			cnt++;
	//	}
	//	if (cnt == 1)
	//		cout << i << endl;
	//}

	//// 1 ~ MAX_NUMBER까지의 소수 개수. 최대한 쓰레드 사용해서

	vector<thread> threads;
	atomic<int> primeCount = 0;
	int coreCount = thread::hardware_concurrency();
	int jobCount = (MAX_NUMBER / coreCount) + 1;

	for (int i = 0; i < coreCount; ++i)
	{
		int start = (i * jobCount) + 1;
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));

		threads.push_back(thread([start, end, &primeCount]() 
			{
				primeCount += CountPrime(start, end);

			}));
	}
	for (thread& t : threads)
		t.join();

	cout << primeCount << endl;
}
