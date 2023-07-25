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
		// ~1024���� 32����, ~2048���� 128, ~4096���� 256����
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

	// �޸� ũ�� <-> �޸� Ǯ
	// O(1) ������ ã�� ���� ���� ���̺�
	// 0 ~ 32
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];

};


template<typename Type, typename... Args> // typename... �̸�
Type* xnew(Args&&... args) // ��������
{
	// �׳� new -> �޸� �Ҵ��ϰ� �����ڸ� ȣ������
	Type* memory = static_cast<Type*>(yalloc(sizeof(Type)));

	// placement new
	new(memory)Type(std::forward<Args>(args)...); // �޸𸮴� �����ϱ� ���� ���������� �޸� �����ٰ� �����ڸ� ȣ�� ����

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xrelease(obj);
}