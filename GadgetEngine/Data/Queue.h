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

		void Remove(){
			GADGET_BASIC_ASSERT(!IsEmpty());
			if(IsEmpty()){
				Debug::ThrowFatalError(SID("DATA"), "Called Pop on an empty Queue!", ErrorCode::Invalid_State, __FILE__, __LINE__);
			}

			T value = Peek();
			data.PopFront();
			return value;
		}

		T Peek() const{
			GADGET_BASIC_ASSERT(!IsEmpty());
			if(IsEmpty()){
				Debug::ThrowFatalError(SID("DATA"), "Called Peek on an empty Queue!", ErrorCode::Invalid_State, __FILE__, __LINE__);
			}

			GADGET_BASIC_ASSERT(data.Front() != nullptr);
			return data.Front()->value;
		}

		bool IsEmpty() const{ return data.IsEmpty(); }
		size_t Size() const{ return data.Size(); }

	private:
		List<T> data;
	};
}

#endif //!GADGET_DATA_QUEUE_H