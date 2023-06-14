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
	// 1) 새 노드를 만든다.
	// 2) 새 노드의 next = head
	// 3) head = 새 노드

	// [ ][ ][ ][ ][ ][ ][ ]
	// [head]

	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head; // 이 head는 여러 스레드가 동시 접근 할 수 있슴.

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

		// 위의 코드를 atomoic하게 만들어줌
		while (_head.compare_exchange_weak(node->next, node) == false)
		{
			//node->next = _head; -> cas에서 해주고 있슴.
		}

		//이 사이에 새치기 당하면?
		//_head = node;

	}


	// 1) head를 읽는다.
	// 2) head의 next를 읽는다.
	// 3) head = head->next로 바꾼다.
	// 4) 처음 head였던 node의 data 추출해서 반환
	// 5) 추출한 노드를 삭제

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
		// 위의 코드를 atomic하게 바꿈
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
			
			//oldHead = _head -> cas에서 해주고 있슴.
		}

		if (oldHead == nullptr)
			return false;

		// Exception X (예외를 신경쓰지 않는다.)
		value = oldHead->data;
		
		// 잠시 삭제 보류
		// 왜? 싱글 스레드에선 문제가 생기지 않는다. 
		// 그러나 멀티 스레드 환경에서 한 스레드가 데이터를 읽고 delete했는데 다른 스레드가 접근해서 head를 읽는다면 이미 delete된 애를 읽게 되는것이다.
		// 그럼 문제가 발생하기때문에 일단 삭제를 보류해보자.
		// delete oldHead;

		return true;
	}


private:
	// [ ][ ][ ][ ][ ][ ][ ]
	// [head]
	atomic<Node*> _head;
	
};