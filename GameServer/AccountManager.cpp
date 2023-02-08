#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void AccountManager::ProcessLogin()
{
	User* user = UserManager::Instance()->GetUser(100);
	// account lock
	lock_guard<mutex> guard(_mutex);

	// userLock

	//TODO


}