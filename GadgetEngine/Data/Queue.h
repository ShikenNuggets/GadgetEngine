#ifndef GADGET_DATA_QUEUE_H
#define GADGET_DATA_QUEUE_H

#include "Data/List.h"

namespace Gadget{
	template <class T>
	class Queue{
	public:
		Queue() : data(){}
		~Queue() = default;

		void Add(const T& value_){
			data.Add(value_);
		}

		T Remove(){
			GADGET_BASIC_ASSERT(!IsEmpty());
			if(IsEmpty()){
				Debug::ThrowFatalError(SID("DATA"), "Called Pop on an empty Queue!", ErrorCode::Invalid_State, __FILE__, __LINE__);
			}

			T value = Peek();
			data.PopFront();
			return value;
		}

		const T& Peek() const{
			GADGET_BASIC_ASSERT(!IsEmpty());
			if(IsEmpty()){
				Debug::ThrowFatalError(SID("DATA"), "Called Peek on an empty Queue!", ErrorCode::Invalid_State, __FILE__, __LINE__);
			}

			GADGET_BASIC_ASSERT(data.Front() != nullptr);
			return data.Front()->value;
		}

		T& Peek(){
			GADGET_BASIC_ASSERT(!IsEmpty());
			if(IsEmpty()){
				Debug::ThrowFatalError(SID("DATA"), "Called Peek on an empty Queue!", ErrorCode::Invalid_State, __FILE__, __LINE__);
			}

			GADGET_BASIC_ASSERT(data.Front() != nullptr);
			return data.Front()->value;
		}

		constexpr inline bool IsEmpty() const{ return data.IsEmpty(); }
		constexpr inline int64_t Size() const{ return data.Size(); }
		constexpr inline size_t SizeInBytes() const{ return data.SizeInBytes(); }

	private:
		List<T> data;
	};
}

#endif //!GADGET_DATA_QUEUE_H