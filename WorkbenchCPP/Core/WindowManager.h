#ifndef GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H
#define GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H

#include <Data/Array.h>
#include <Data/List.h>

#include "Windows/SubWindow.h"

namespace Gadget::Workbench{
	class WindowManager{
	public:
		WindowManager();

		void AddWindow(SubWindow* window_);

		template <class T, typename... Args>
		T* AddWindow(Args... args_){
			static_assert(std::is_base_of_v<SubWindow, T>, "T must derive from SubWindow");
			T* subWnd = new T(args_...);
			GADGET_BASIC_ASSERT(subWnd != nullptr);
			windows.Add(subWnd);
			return subWnd;
		}

		void RemoveWindow(SubWindow* window_);

		//This is slower than the other override, use the pointer if you have it!
		template <class T>
		void RemoveWindow(){
			for(const auto* w : windows){
				auto* window = dynamic_cast<T*>(w);
				if(window != nullptr){
					removeList.Add(window);
					return;
				}
			}
		}

		void Update();

	private:
		Array<SubWindow*> windows;
		List<SubWindow*> removeList;
	};
}

#endif //!GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H