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
		_condVar.notify_one(); // ����ϴ� �ָ� ����
	}


	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;
		// top�� ���� ������ �ܺη� �����͸� �������� ������ ũ������ ���ü� �ֽ� ���ܰ� �߻���.
		// �޸𸮰� �����ؼ� ���� �������� �ͼ���(����)�� ����� �ڷᱸ���� �� ������ ������ top�� ��

		//empty -> top -> pop 
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}


	void WaitPop(T& value)
	{
		// unique_lock�� ���� ���� ����Ǻ������ ������ �� �� ���������� ���� �����ؾ��ϱ� ������
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; });

		value = std::move(_stack.top());
		_stack.pop();
	}
	//bool Empty() 
	//// ���� �ϱ����� empty���� Ȯ���ؾ���. ��Ƽ������ ȯ�濡���� emptyȮ�� �� pop�� �Ѵٴ°� �ƹ� �ǹ̰� ����
	//{
	//	lock_guard<mutex> lock(_mutex);
	//	return _stack.empty();

	//}



private:
	stack<T> _stack;
	mutex _mutex;

	condition_variable _condVar;
};
