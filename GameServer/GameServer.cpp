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
	// 가상 메모리 기본
	
	// 유저레벨 (메모장, 롤, 서버) 여러 프로그램 띄움
	//----------------------------
	// 커널레벨 (OS CODE)

	// 2GB [						] 
	// 옵션 어떤 부분은 접근,사용 가능 / 접근은 가능하지만 WRITE불가 / 아무렇게 접근 가능
	// 2GB / 4kb [ooooxxxxoooxxoxxxox	] 
	// 2GB / 4kb [[r][w][rw][x][][][][][][]  ] 페이지 단위로 관리
	// 
	//Knight* test = (Knight*)::VirtualAlloc(NULL, 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//test->_hp = 100;
	//::VirtualFree(test, 0, MEM_RELEASE); // 이제 이 영역은 사용할 수 없는 영역
	//test->_hp = 200;

	// 오버플로우 문제
	// [                    [     ]] 끝에부터 메모리를 저장

	Knight* knight = (Knight*)xnew<Player>();

	knight->_hp = 100;
	xdelete(knight);

}
