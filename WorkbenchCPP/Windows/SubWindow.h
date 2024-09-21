#ifndef GADGET_WORKBENCH_WINDOWS_SUB_WINDOW_H
#define GADGET_WORKBENCH_WINDOWS_SUB_WINDOW_H

#include <Data/String.h>

namespace Gadget::Workbench{
	class SubWindow{
	public:
		SubWindow(String name_) : name(std::move(name_)){}
		virtual ~SubWindow() = default;

		virtual void Draw() = 0;

	protected:
		String name;
	};
}

#endif //!GADGET_WORKBENCH_WINDOWS_SUB_WINDOW_H