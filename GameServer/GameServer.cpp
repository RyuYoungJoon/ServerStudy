#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <windows.h>
#include <future>

// 문제의 원인
// 가시성, 재배치

// 가시성
// cpu가 어떤 값을 쓰거나 읽을 때 곧이 곧대로 ram까지 100%확률로 가지 않음
// cpu에서 각 코어마다 자기만의 캐시를 갖고 있슴
// 
// 코드재배치
// 우리는 cpu와 컴파일러에게 속고 있었다.
// 우리가 작업한 코드를 컴파일러가 그대로 번역하지 않을 수 있다.
// 컴파일러가 검수할 때 순서를 바꾸면 속도가 더 빠르지 않을까라고 판단하면 코드를 재배치함.
// 컴파일러가 안하더라고 cpu가 바꿀수있슴 ㅡㅡ
// 
//
int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

volatile bool ready;

void Thread_1()
{
	while (!ready);
	y = 1;	// Store y
	r1 = x; // Load x
	
}

void Thread_2()
{
	while (!ready);

	x = 1;	// Store x
	r2 = y; // Load y
}
int main()
{
	int32 count = 0;

	while (true)
	{
		ready = false;

		count++;

		x = y = r1 = r2 = 0;

		thread t1(Thread_1);
		thread t2(Thread_2);

		ready = true;

		t1.join();
		t2.join();

		if (r1 == 0 && r2 == 0)
			break;
	}
	cout << count << " 번만에 빠져나옴" << endl;
}
