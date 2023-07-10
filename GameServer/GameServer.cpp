#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "AccountManager.h"
#include "PlayerManager.h"

int main()
{

	GThreadManger->Launch([=]
		{
			while (true) {
				cout << "PlayerThenAccount" << endl;
				GPlayerManager.PlayerThenAccount();
				this_thread::sleep_for(100ms);
			}
		});
	
	GThreadManger->Launch([=]
		{
			while (true) {
				cout << "AccoutThenPlayer" << endl;
				GAccountManager.AccountThenPlayer();
				this_thread::sleep_for(100ms);
			}
		});
	
	

	GThreadManger->Join();
}
