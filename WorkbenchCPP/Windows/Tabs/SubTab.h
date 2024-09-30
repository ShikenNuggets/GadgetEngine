#ifndef GADGET_WORKBENCH_WINDOWS_TABS_SUB_TAB_H
#define GADGET_WORKBENCH_WINDOWS_TABS_SUB_TAB_H

namespace Gadget::Workbench{
	class SubTab{
	public:
		SubTab() = default;

		virtual void Update(){}
		virtual void Draw(){}
	};
}

#endif //!GADGET_WORKBENCH_WINDOWS_TABS_SUB_TAB_H