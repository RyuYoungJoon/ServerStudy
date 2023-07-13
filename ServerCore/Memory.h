#pragma once
#include "Allocator.h"

template<typename Type, typename... Args> // typename... 이름
Type* xnew(Args&&... args) // 가변길이
{
	// 그냥 new -> 메모리 할당하고 생성자를 호출해줘
	Type* memory = static_cast<Type*>(xalloc(sizeof(Type)));

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