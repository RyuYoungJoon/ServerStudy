#pragma once

#include <mutex>

template<typename T>
class LockQueue
{
public:
	LockQueue(){}

	LockQueue(const LockQueue&) = delete;
	LockQueue& operator=(const LockQueue&) = delete;
	void Push(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		_queue.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_queue.empty())
			return false;
		// top을 쓰는 이유는 외부로 데이터를 꺼내오는 순간에 크래쉬가 나올수 있슴 예외가 발생함.
		// 메모리가 부족해서 복사 과정에서 익셉션(예외)가 생기면 자료구조가 다 깨지기 때문에 top을 씀

		//empty -> top -> pop 
		value = std::move(_queue.front());
		_queue.pop();
		return true;
	}


	void WaitPop(T& value)
	{
		// unique_lock을 쓰는 이유 컨디션베리어블 웨잇을 할 때 내부적으로 락을 해제해야하기 떄문에
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _queue.empty() == false; });

		value = std::move(_queue.front());
		_queue.pop();
		return true;
	}

private:
	queue<T> _queue;
	mutex _mutex;
	condition_variable _condVar;


};

