#pragma once
#include "Allocator.h"

class MemoryPool;

/*------------------
	MemoryManger
--------------------*/

class MemoryManager
{
	enum
	{
		// ~1024까지 32단위, ~2048까지 128, ~4096까지 256단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	MemoryManager();
	~MemoryManager();

	void* Allocate(int32 size);
	void Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기 <-> 메모리 풀
	// O(1) 빠르게 찾기 위한 헬퍼 테이블
	// 0 ~ 32
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];

};


template<typename Type, typename... Args> // typename... 이름
Type* xnew(Args&&... args) // 가변길이
{
	// 그냥 new -> 메모리 할당하고 생성자를 호출해줘
	Type* memory = static_cast<Type*>(yalloc(sizeof(Type)));

	// placement new
	new(memory)Type(std::forward<Args>(args)...); // 메모리는 있으니까 굳이 만들지말고 메모리 위에다가 생성자를 호출 해줘

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xrelease(obj);
}