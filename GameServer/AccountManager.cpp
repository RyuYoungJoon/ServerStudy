#include "pch.h"
#include "AccountManager.h"
#include "PlayerManager.h"

AccountManger GAccountManager;

void AccountManger::AccountThenPlayer()
{
	WRITE_LOCK;
	GPlayerManager.Lock();

}

void AccountManger::Lock()
{
	WRITE_LOCK;
}
