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


template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value)
		{};
		T data;
		Node* next;

	};

public:
	// 1) �� ��带 �����.
	// 2) �� ����� next = head
	// 3) head = �� ���

	// [ ][ ][ ][ ][ ][ ][ ]
	// [head]

	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head; // �� head�� ���� �����尡 ���� ���� �� �� �ֽ�.

		/*if (_head == node->next)
		{
			_head = node;
			return true;
		}
		else
		{
			node->next = _head;
			return false;
		}*/

		// ���� �ڵ带 atomoic�ϰ� �������
		while (_head.compare_exchange_weak(node->next, node) == false)
		{
			//node->next = _head; -> cas���� ���ְ� �ֽ�.
		}

		//�� ���̿� ��ġ�� ���ϸ�?
		//_head = node;

	}


	// 1) head�� �д´�.
	// 2) head�� next�� �д´�.
	// 3) head = head->next�� �ٲ۴�.
	// 4) ó�� head���� node�� data �����ؼ� ��ȯ
	// 5) ������ ��带 ����

	bool TryPop(T& value)
	{
		Node* oldHead = _head;
		
		/*if (_head == oldHead)
		{
			_head = oldHead->next;
			return true;
		}
		else
		{
			oldHead = _head;
			return false;

		}*/
		// ���� �ڵ带 atomic�ϰ� �ٲ�
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
			
			//oldHead = _head -> cas���� ���ְ� �ֽ�.
		}

		if (oldHead == nullptr)
			return false;

		// Exception X (���ܸ� �Ű澲�� �ʴ´�.)
		value = oldHead->data;
		
		// ��� ���� ����
		// ��? �̱� �����忡�� ������ ������ �ʴ´�. 
		// �׷��� ��Ƽ ������ ȯ�濡�� �� �����尡 �����͸� �а� delete�ߴµ� �ٸ� �����尡 �����ؼ� head�� �д´ٸ� �̹� delete�� �ָ� �а� �Ǵ°��̴�.
		// �׷� ������ �߻��ϱ⶧���� �ϴ� ������ �����غ���.
		// delete oldHead;

		return true;
	}


private:
	// [ ][ ][ ][ ][ ][ ][ ]
	// [head]
	atomic<Node*> _head;
	
};