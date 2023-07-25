#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"



ThreadManager*		GThreadManger = nullptr;
MemoryManager*		GMemory = nullptr;
DeadLockProfiler*	GDeadLockProfiler = nullptr;


class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManger = new ThreadManager();
		GMemory = new MemoryManager();
		GDeadLockProfiler = new DeadLockProfiler();
	}
	~CoreGlobal()
	{
		delete GThreadManger;
		delete GMemory;
		delete GDeadLockProfiler;
	}


} GCoreGlobal;

