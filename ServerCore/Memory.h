#pragma once
#include "Allocator.h"

template<typename Type, typename... Args> // typename... �̸�
Type* xnew(Args&&... args) // ��������
{
	// �׳� new -> �޸� �Ҵ��ϰ� �����ڸ� ȣ������
	Type* memory = static_cast<Type*>(xalloc(sizeof(Type)));

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