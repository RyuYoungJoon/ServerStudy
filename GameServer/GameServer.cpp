#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>

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
	// 쓰레드 객체
	vector<thread> v;

	v.resize(10);

	thread t;
	auto id1 = t.get_id();
	
	t = thread(HelloThread);

	cout << "Hello Main" << endl;
	// 이 다섯개만 알고있자.
	int32 cnt = t.hardware_concurrency(); // cpu 코어의 개수?
	
	auto id2 = t.get_id();		// 쓰레드마다 갖고 있는 id 1 2 3 4 로 늘어나지 않고 들쑥날쑥 좆같은 숫자이지만 구분은 됨
	
	//t.detach();		// thread객체에서 실제 쓰레드를 분리 (연결고리를 끊어버림) 앵간해서는 거의 안씀 알고만 있자.
	
	if(t.joinable())	 
		t.join();

}
