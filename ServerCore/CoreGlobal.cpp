#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManger = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		cout << "������ ����" << endl;
		GThreadManger = new ThreadManager();
	}
	~CoreGlobal()
	{
		cout << "������ ����" << endl;
		delete GThreadManger;
	}


} GCoreGlobal;

