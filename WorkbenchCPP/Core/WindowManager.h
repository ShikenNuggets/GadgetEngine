#ifndef GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H
#define GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H

#include <Data/Array.h>

#include "Windows/SubWindow.h"

namespace Gadget::Workbench{
	class WindowManager{
	public:
		WindowManager();

		void AddWindow(SubWindow* window_);

		void Update();

	private:
		Array<SubWindow*> windows;
	};
}

#endif //!GADGET_WORKBENCH_CORE_WINDOW_MANAGER_H