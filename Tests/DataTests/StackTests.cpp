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
	int64_t Size() const{ return size; }
	int64_t NumStacks() const{ return stacks.Size(); }

private:
	Stack<Stack<T>> stacks;
	int64_t size;

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

//------------------------------------------------------------//
//----------------- Sort Stack (CTCI 3.4) --------------------//
//------------------------------------------------------------//

static inline void SortStack(Stack<int>& stack){
	if(stack.IsEmpty()){
		return;
	}

	Stack<int> tempStack;
	while(!stack.IsEmpty()){
		tempStack.Push(stack.Pop());
	}

	while(!tempStack.IsEmpty()){
		int temp = tempStack.Pop();

		while(!stack.IsEmpty() && stack.Peek() < temp){
			tempStack.Push(stack.Pop());
		}

		stack.Push(temp);
	}
}

TEST_CASE("Stack Sort", "[stack_sort]"){
	Stack<int> stack;	REQUIRE(stack.IsEmpty());
	stack.Push(5);		REQUIRE(stack.Size() == 1); REQUIRE(stack.Peek() == 5);
	stack.Push(1);		REQUIRE(stack.Size() == 2); REQUIRE(stack.Peek() == 1);
	stack.Push(2);		REQUIRE(stack.Size() == 3); REQUIRE(stack.Peek() == 2);
	stack.Push(3);		REQUIRE(stack.Size() == 4); REQUIRE(stack.Peek() == 3);
	stack.Push(4);		REQUIRE(stack.Size() == 5); REQUIRE(stack.Peek() == 4);

	SortStack(stack);

	REQUIRE(stack.Size() == 5);
	REQUIRE(stack.Pop() == 1);	REQUIRE(stack.Size() == 4);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 2);	REQUIRE(stack.Size() == 3);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 3);	REQUIRE(stack.Size() == 2);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 4);	REQUIRE(stack.Size() == 1);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 5);	REQUIRE(stack.Size() == 0);	REQUIRE(stack.IsEmpty());

	//--------------------------------------------------//

	stack.Push(5);	REQUIRE(stack.Size() == 1); REQUIRE(stack.Peek() == 5);
	stack.Push(6);	REQUIRE(stack.Size() == 2); REQUIRE(stack.Peek() == 6);
	stack.Push(4);	REQUIRE(stack.Size() == 3); REQUIRE(stack.Peek() == 4);
	stack.Push(1);	REQUIRE(stack.Size() == 4); REQUIRE(stack.Peek() == 1);
	stack.Push(9);	REQUIRE(stack.Size() == 5); REQUIRE(stack.Peek() == 9);

	SortStack(stack);

	REQUIRE(stack.Size() == 5);
	REQUIRE(stack.Pop() == 1);	REQUIRE(stack.Size() == 4);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 4);	REQUIRE(stack.Size() == 3);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 5);	REQUIRE(stack.Size() == 2);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 6);	REQUIRE(stack.Size() == 1);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 9);	REQUIRE(stack.Size() == 0);	REQUIRE(stack.IsEmpty());

	//--------------------------------------------------//

	stack.Push(1);	REQUIRE(stack.Size() == 1); REQUIRE(stack.Peek() == 1);
	stack.Push(3);	REQUIRE(stack.Size() == 2); REQUIRE(stack.Peek() == 3);
	stack.Push(8);	REQUIRE(stack.Size() == 3); REQUIRE(stack.Peek() == 8);
	stack.Push(12);	REQUIRE(stack.Size() == 4); REQUIRE(stack.Peek() == 12);
	stack.Push(5);	REQUIRE(stack.Size() == 5); REQUIRE(stack.Peek() == 5);
	stack.Push(10);	REQUIRE(stack.Size() == 6); REQUIRE(stack.Peek() == 10);
	stack.Push(17);	REQUIRE(stack.Size() == 7); REQUIRE(stack.Peek() == 17);

	SortStack(stack);

	REQUIRE(stack.Size() == 7);
	REQUIRE(stack.Pop() == 1);	REQUIRE(stack.Size() == 6);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 3);	REQUIRE(stack.Size() == 5);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 5);	REQUIRE(stack.Size() == 4);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 8);	REQUIRE(stack.Size() == 3);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 10);	REQUIRE(stack.Size() == 2);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 12);	REQUIRE(stack.Size() == 1);	REQUIRE(!stack.IsEmpty());
	REQUIRE(stack.Pop() == 17);	REQUIRE(stack.Size() == 0);	REQUIRE(stack.IsEmpty());
}