#include <Gadget.h>
#include <Data/Stack.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//--------------- Stack of Plates (CTCI 3.3) -----------------//
//------------------------------------------------------------//
template <typename T, int _Capacity>
class StackOfStacks{
public:
	StackOfStacks() : stacks(), size(0){}
	~StackOfStacks() = default;

	void Push(const T& value_){
		if(size == 0 || TopStack().Size() >= _Capacity){
			stacks.Push(Stack<T>());
		}

		TopStack().Push(value_);
		size++;
	}

	T Pop(){
		T value = TopStack().Peek();
		TopStack().Pop();
		if(TopStack().IsEmpty()){
			stacks.Pop();
		}
		
		size--;
		return value;
	}

	const T& Peek() const{ return TopStack().Peek(); }
	T& Peek(){ return TopStack().Peek(); }

	bool IsEmpty() const{ return size == 0; }
	size_t Size() const{ return size; }
	size_t NumStacks() const{ return stacks.Size(); }

private:
	Stack<Stack<T>> stacks;
	size_t size;

	Stack<T>& TopStack(){
		GADGET_BASIC_ASSERT(!stacks.IsEmpty());
		return stacks.Peek();
	}
};

TEST_CASE("Stack Set of Stacks" "[stack_set_of_stacks]"){
	StackOfStacks<int, 3> set;	REQUIRE(set.IsEmpty());
	set.Push(1);				REQUIRE(set.Size() == 1);	REQUIRE(set.Peek() == 1);	REQUIRE(set.NumStacks() == 1);
	set.Push(2);				REQUIRE(set.Size() == 2);	REQUIRE(set.Peek() == 2);	REQUIRE(set.NumStacks() == 1);
	set.Push(3);				REQUIRE(set.Size() == 3);	REQUIRE(set.Peek() == 3);	REQUIRE(set.NumStacks() == 1);
	set.Push(4);				REQUIRE(set.Size() == 4);	REQUIRE(set.Peek() == 4);	REQUIRE(set.NumStacks() == 2);
	set.Push(5);				REQUIRE(set.Size() == 5);	REQUIRE(set.Peek() == 5);	REQUIRE(set.NumStacks() == 2);
	set.Push(6);				REQUIRE(set.Size() == 6);	REQUIRE(set.Peek() == 6);	REQUIRE(set.NumStacks() == 2);
	set.Push(7);				REQUIRE(set.Size() == 7);	REQUIRE(set.Peek() == 7);	REQUIRE(set.NumStacks() == 3);

	set.Pop();					REQUIRE(set.Size() == 6);	REQUIRE(set.Peek() == 6);	REQUIRE(set.NumStacks() == 2);
	set.Pop();					REQUIRE(set.Size() == 5);	REQUIRE(set.Peek() == 5);	REQUIRE(set.NumStacks() == 2);
	set.Pop();					REQUIRE(set.Size() == 4);	REQUIRE(set.Peek() == 4);	REQUIRE(set.NumStacks() == 2);
	set.Pop();					REQUIRE(set.Size() == 3);	REQUIRE(set.Peek() == 3);	REQUIRE(set.NumStacks() == 1);
}

//------------------------------------------------------------//
//--------------- Queue of Stacks (CTCI 3.4) -----------------//
//------------------------------------------------------------//

template <typename T>
class QueueOfStacks{
public:
	QueueOfStacks() : stackNew(), stackOld(){}
	~QueueOfStacks() = default;

	void Add(const T& value_){
		stackNew.Push(value_);
	}

	T Remove(){
		ShiftStacks();
		return stackOld.Pop();
	}

	const T& Peek(){
		ShiftStacks();
		return stackOld.Peek();
	}

	size_t Size() const{ return stackNew.Size() + stackOld.Size(); }
	bool IsEmpty() const{ return Size() == 0; }

private:
	Stack<T> stackNew;
	Stack<T> stackOld;

	void ShiftStacks(){
		if(!stackOld.IsEmpty()){
			return; //If we shift over while the old stack has elements, the order will get messed up
		}

		//Pushing all the new elements into the old stack reverses the order to allow for FIFO
		while(!stackNew.IsEmpty()){
			stackOld.Push(stackNew.Pop());
		}
	}
};

TEST_CASE("Stack QueueOfStacks", "[stack_queue_of_stacks]"){
	QueueOfStacks<int> queue;	REQUIRE(queue.IsEmpty());

	queue.Add(1);	REQUIRE(queue.Size() == 1);	REQUIRE(queue.Peek() == 1);
	queue.Add(2);	REQUIRE(queue.Size() == 2);	REQUIRE(queue.Peek() == 1);
	queue.Add(3);	REQUIRE(queue.Size() == 3);	REQUIRE(queue.Peek() == 1);
	queue.Add(4);	REQUIRE(queue.Size() == 4);	REQUIRE(queue.Peek() == 1);
	queue.Add(5);	REQUIRE(queue.Size() == 5);	REQUIRE(queue.Peek() == 1);

	queue.Remove();	REQUIRE(queue.Size() == 4); REQUIRE(queue.Peek() == 2);
	queue.Remove();	REQUIRE(queue.Size() == 3); REQUIRE(queue.Peek() == 3);
	queue.Remove();	REQUIRE(queue.Size() == 2); REQUIRE(queue.Peek() == 4);
	queue.Remove();	REQUIRE(queue.Size() == 1); REQUIRE(queue.Peek() == 5);
	queue.Remove();	REQUIRE(queue.Size() == 0);
}