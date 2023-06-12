#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <windows.h>
#include <future>

atomic<bool> flag;

int main()
{
	flag.store(false);

	// 왜 락이 없냐? 락이 없어도 원자적으로 처리 할 수 있기때문에 true
	// false면 원자적으로 처리하려면 락이 피료여하다.
	
	//flag.is_lock_free();
	

	//flag = true;
	flag.store(true,memory_order::memory_order_seq_cst);

	// bool val = flag;
	bool val = flag.load(memory_order::memory_order_seq_cst);

	// 이전 flag 값을 prev에 넣고, flag 값을 수정
	{

		// 이건 좀 문제가 있다.
		//bool prev = flag;
		//flag = true;
		// flag값을 prev에 넣었다고 생각하겠지만. 순간 flag값을 다른 스레드가 접근했다고 생각하면
		// 이 값은 잘못된 값이다.
		// 결국 저 2줄의 코드가 원자적으로 한번에 이뤄져야함.
		// 읽고 쓰는것을 한번에 해주는 놈이 있다.
		// exchange
		// 넣기 직전에 값을 저장해주는것
		bool prev = flag.exchange(true);

	}

	// CAS (Compare-And-Swap) 조건부 수정
	{
		// 만약 내가 기대하는 값이 expected라면 desire로 바꿔라
		bool expected = false;
		bool desired = true;
		flag.compare_exchange_strong(expected, desired);

		// 위에 코드는 이 뜻이다.
		// 이 코드를 원자적으로 해주는 cas
		/*if (flag == expected)
		{
			flag = desired;
			flag = true;
		}
		else
		{
			expected = flag;
			return false;
		}*/

		// strong과 weak는 거의 똑같음
		//while (true) {
		//	bool exptected = false;
		//	bool desired = true;
		//	flag.compare_exchange_weak(expected, desired);
		//}
		// 하나 차이가 있다.
		// Spurious Failure (가짜 실패)
		// 동작은 똑같지만 만약 다른 쓰레드의 interruption을 받아서 중간에 실패한다면
		// false를 반환 할 수도 있다.
		// 근데 strong은 성공 할 때 까지 반복해서 true를 받아낸다.
		// weak는 while과 같이 쓴다.
	}
}
