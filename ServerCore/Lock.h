#pragma once
#include "Types.h"


/*--------------------
	RW SpinLock
----------------------*/

/*----------------------------------------------

[WWWWWWWW][WWWWWWWW] [RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exculsive Lock Owner ThreadId)
R : ReadFlag (Shared Lock Count)
------------------------------------------------*/

// W W W 연속 가능 W 잡은 상태에서 R 잡기 가능
// 근데 R -> W는 불가
class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock();
	void WriteUnlock();

	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount;
};

/*------------------------
		LockGuard
--------------------------*/

// RAII
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnlock(); }
private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnlock(); }
private:
	Lock& _lock;
};