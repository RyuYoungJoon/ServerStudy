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
		Node(const T& value) : data(value), next(nullptr)
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
		++_popCount;

		Node* oldHead = _head;
		
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{}

		if (oldHead == nullptr)
		{
			--_popCount;
			return false;
		}
		// Exception X (예외를 신경쓰지 않는다.)
		value = oldHead->data;
		TryDelete(oldHead);

		return true;
	}

	// 1) 데이터 분리
	// 2) Count 체크
	// 3) 나 혼자면 삭제
	void TryDelete(Node* oldHead)
	{
		// 나 외에 누가 있는가?
		if (_popCount == 1) // 나 혼자다.
		{
			// 이왕 혼자인거, 삭제 예약된 다른 데이터들도 삭제해보자.
			Node* node = _pendingList.exchange(nullptr);

			if (--_popCount == 0)
			{
				// 중간에 끼어든 놈이 없다 -> 삭제 진행
				// 이제와서 끼어들어도 어쩌피 데이터는 분리해둔 상태
				DeleteNodes(node);
			}
			else if(node)
			{
				// 누가 끼어 들었으니 다시 갖다 놓자.
				ChainPendingNodeList(node);
			}

			// 내 데이터는 삭제
			delete oldHead;
		}
		else // 누군가가 trypop에 들어왔다.
		{
			// 누가 있네? 그럼 지금 삭제 하지 않고, 삭제 예약만
			ChainPendingNode(oldHead);
			--_popCount;

		}

	}
	// [ ][ ][ ][ ][ ][ ]
	
	void ChainPendingNodeList(Node* first, Node* last)
	{
		// [ ][ ][ ][ ][ ][ ] -> [ ][ ][ ][ ][ ] PENDINGLIST 추가

		last->next = _pendingList;

		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{}

	}
	// [ ][ ][ ][ ][ ][ ][ ]
	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		while (last->next)
			last = last->next;

		ChainPendingNodeList(node, last);

	}
	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}

	static void DeleteNodes(Node* node)
	{
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}

private:
	atomic<Node*> _head;
	
	atomic<uint32> _popCount = 0; // Pop을 실행중인 쓰레드 개수
	atomic<Node*> _pendingList; // 삭제 되어야할 노드들 (첫번째 노드)
};