#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManger = nullptr;

CoreGlobal::CoreGlobal()
{
	cout << "������ ����" << endl;
	GThreadManger = new ThreadManager();
}

CoreGlobal::~CoreGlobal()
{
	cout << "������ ����" << endl;
	delete GThreadManger;
}
