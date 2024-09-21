#ifndef GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H
#define GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H

#include <Data/Array.h>

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

		void Update();

	private:
		Array<SubWindow*> windows;
	};
}

#endif //!GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H