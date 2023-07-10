#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"

ThreadManager* GThreadManger = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		cout << "스레드 생성" << endl;
		GThreadManger = new ThreadManager();
		GDeadLockProfiler = new DeadLockProfiler();
	}
	~CoreGlobal()
	{
		cout << "스레드 삭제" << endl;
		delete GThreadManger;
		delete GDeadLockProfiler;
	}


} GCoreGlobal;

