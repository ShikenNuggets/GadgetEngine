#ifndef GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H
#define GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H

#include "Windows/SubWindow.h"
#include "Windows/Tabs/SubTab.h"

namespace Gadget::Workbench{
	class EditorToolbar : public SubWindow{
	public:
		enum class Tab : uint8_t{
			None = 0,
			Scenes,
			Objects,
			Resources,

			Tab_MAX
		};

		EditorToolbar();

		virtual void Draw() override;

		Tab GetCurrentTab() const{ return currentTab; }

	private:
		Tab currentTab;
		StaticArray<SubTab*, static_cast<int64_t>(Tab::Tab_MAX)> tabs;
	};
}

#endif //!GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H