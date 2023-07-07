#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManger = nullptr;

CoreGlobal::CoreGlobal()
{
	cout << "스레드 생성" << endl;
	GThreadManger = new ThreadManager();
}

CoreGlobal::~CoreGlobal()
{
	cout << "스레드 삭제" << endl;
	delete GThreadManger;
}
