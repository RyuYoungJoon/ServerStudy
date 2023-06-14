#pragma once
#include <mutex>

template<typename T>
class LockStack 
{
public:
	LockStack() {}
	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one(); // 대기하는 애를 깨움
	}


	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;
		// top을 쓰는 이유는 외부로 데이터를 꺼내오는 순간에 크래쉬가 나올수 있슴 예외가 발생함.
		// 메모리가 부족해서 복사 과정에서 익셉션(예외)가 생기면 자료구조가 다 깨지기 때문에 top을 씀

		//empty -> top -> pop 
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}


	void WaitPop(T& value)
	{
		// unique_lock을 쓰는 이유 컨디션베리어블 웨잇을 할 때 내부적으로 락을 해제해야하기 떄문에
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; });

		value = std::move(_stack.top());
		_stack.pop();
	}
	//bool Empty() 
	//// 팝을 하기전에 empty인지 확인해야함. 멀티스레드 환경에서는 empty확인 후 pop을 한다는건 아무 의미가 없음
	//{
	//	lock_guard<mutex> lock(_mutex);
	//	return _stack.empty();

	//}



private:
	stack<T> _stack;
	mutex _mutex;

	condition_variable _condVar;
};
