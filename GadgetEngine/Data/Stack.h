#ifndef GADGET_DATA_STACK_H
#define GADGET_DATA_STACK_H

#include "Debug.h"
#include "Data/List.h"

namespace Gadget{
	template <class T>
	class Stack{
	public:
		Stack() : data(){}
		~Stack() = default;

		void Push(const T& item_){
			data.AddFront(item_);
		}

		T Pop(){
			GADGET_BASIC_ASSERT(!data.IsEmpty());
			if(data.IsEmpty()){
				Debug::ThrowFatalError(SID("DATA"), "Called Pop on an empty stack!", ErrorCode::Invalid_State, __FILE__, __LINE__);
			}

			GADGET_BASIC_ASSERT(data.Front() != nullptr);
			T value = data.Front()->value;
			data.PopFront();
			return value;
		}

		const T& Peek() const{
			GADGET_BASIC_ASSERT(!data.IsEmpty());
			if(data.IsEmpty()){
				Debug::ThrowFatalError(SID("DATA"), "Called Peek on an empty stack!", ErrorCode::Invalid_State, __FILE__, __LINE__);
			}

			GADGET_BASIC_ASSERT(data.Front() != nullptr);
			return data.Front()->value;
		}

		T& Peek(){
			GADGET_BASIC_ASSERT(!data.IsEmpty());
			if(data.IsEmpty()){
				Debug::ThrowFatalError(SID("DATA"), "Called Peek on an empty stack!", ErrorCode::Invalid_State, __FILE__, __LINE__);
			}

			GADGET_BASIC_ASSERT(data.Front() != nullptr);
			return data.Front()->value;
		}

		bool IsEmpty() const{ return data.IsEmpty(); }
		int64_t Size() const{ return data.Size(); }

	private:
		List<T> data;
	};
}

#endif //!GADGET_DATA_STACK_H