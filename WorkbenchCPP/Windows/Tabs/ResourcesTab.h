#ifndef GADGET_WORKBENCH_WINDOWS_TABS_RESOURCES_TAB_H
#define GADGET_WORKBENCH_WINDOWS_TABS_RESOURCES_TAB_H

#include "Windows/Tabs/SubTab.h"

namespace Gadget::Workbench{
	class ResourcesTab : public SubTab{
	public:
		ResourcesTab();

		virtual void Update() override;
		virtual void Draw() override;
	};
}

#endif //!GADGET_WORKBENCH_WINDOWS_TABS_RESOURCES_TAB_H