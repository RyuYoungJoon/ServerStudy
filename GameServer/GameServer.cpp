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

class Knight
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

	/*static void* operator new(size_t size)
	{
		cout << "Knight new! " << size << endl;
		void* ptr = ::malloc(size);
		return ptr;
	}
	static void operator delete(void* ptr)
	{
		cout << "Knight delete" << endl;
		::free(ptr);
	}*/

	int32 _hp = 100;
	int32 _mp = 100;
};

// new operator overloading (Global)
//void* operator new(size_t size)
//{
//	cout << "new! " << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//void operator delete(void* ptr)
//{
//	cout << "delete" << endl;
//	::free(ptr);
//}
//void* operator new[](size_t size)
//{
//	cout << "new[]! " << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//void operator delete[](void* ptr)
//{
//	cout << "delete[]" << endl;
//	::free(ptr);
//}

int main()
{
	// 커널모드까지 가지 않ㄹ고 유저 모드에서 메모리를 할당할 수 있슴
	// 자잘한 메모리들이 할당 되고 free 되면 합쳐지지 않고 그 만큼 낭비가 됨
	//
	Knight* knight = xnew<Knight>(100);

	xdelete(knight);

}
