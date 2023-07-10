#pragma once

class AccountManger
{
	USE_LOCK;
public:
	void AccountThenPlayer();
	void Lock();
};


extern AccountManger GAccountManager;
