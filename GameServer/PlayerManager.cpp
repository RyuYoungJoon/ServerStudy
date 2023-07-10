#include "pch.h"
#include "PlayerManager.h"
#include "AccountManger.h"


PlayerManager GPlayerManager;


void PlayerManager::PlayerThenAccount()
{
	WRITE_LOCK;


	GAccountManager.Lock();
}


void PlayerManager::Lock()
{
	WRITE_LOCK;
}