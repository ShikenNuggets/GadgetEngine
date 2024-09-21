#ifndef GADGET_WORKBENCH_CORE_CALLBACK_H
#define GADGET_WORKBENCH_CORE_CALLBACK_H

#include <functional>

namespace Gadget::Workbench{
	class Callback{
	public:
		Callback(std::function<void(void)> func_) : callbackFunc(func_){}

		template <typename T>
		Callback(T&& func_) : callbackFunc(std::forward<T>(func_)){}

		void operator()(){
			GADGET_BASIC_ASSERT(IsValid());
			if(!IsValid()){
				Debug::PopupErrorMessage("Error", "Callback handler not set!");
			}

			return callbackFunc();
		}

		inline bool IsValid() const{ return callbackFunc != nullptr; }

	private:
		std::function<void(void)> callbackFunc;
	};
}

#endif //!GADGET_WORKBENCH_CORE_CALLBACK_H