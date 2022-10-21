#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <mutex>
void HelloThread()
{
	cout << "헬로 쓰레드" << endl;
}
void HelloThread_2(int32 num)
{
	cout << num << endl;


}
int main()
{
	vector<thread> v;

	for (int32 i = 0; i < 10; ++i) {
		v.push_back(thread(HelloThread_2, i));
	}

	for (int32 i = 0; i < 10; ++i) {
		if (v[i].joinable())
			v[i].join();
	}

	cout << "헬로 메인" << endl;
	
}
