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
using KnightRef = TSharedPtr<class Knight>;
using InventoryRef = TSharedPtr<class Inventory>;

class Knight
{
public:
	Knight()
	{
		cout << "생성자" << endl;
	}

	~Knight()
	{
		cout << "소멸자" << endl;
	}

};


int main()
{
	// 1) 이미 만들어진 클래스 대상으로 사용 불가
	// 2) 순환 (Cycle) 문제


	/*unique_ptr<Knight> k2 = make_unique<Knight>();
	unique_ptr<Knight> k3 = std::move(k2);*/
	// unique_ptr -> 복사를 막아놨슴 k2 = k3 오류
	
	// shared_ptr
	// weak_ptr 순환문제 해결 가능

	// [knight][RefCountingBlock]
	
	// [T*][RefCountBlocking*]

	// RefCountBlock(useCount(shared), weakCount)
	shared_ptr<Knight> spr = make_shared<Knight>();
	weak_ptr<Knight> wpr = spr; // 객체가 사라지는건 상관 안함. 
	//RCB를 사용하여 객체가 진짜 살아졌는지 안했는지를 테스트하기 위한 부분을 유지시켜주는 역할 

	bool expired = wpr.expired(); // wpr이 존재하는지 체크

	shared_ptr<Knight>spr2 = wpr.lock(); // 위에 방법이 번거롭다면 그냥 캐스팅
	if (spr2 != nullptr)
	{

	}
}
