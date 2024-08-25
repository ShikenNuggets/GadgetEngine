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