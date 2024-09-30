#ifndef GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H
#define GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H

#include "Windows/SubWindow.h"

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

		EditorToolbar() : SubWindow("Toolbar"), currentTab(Tab::None){}

		virtual void Draw() override;

		Tab GetCurrentTab() const{ return currentTab; }

	private:
		Tab currentTab;
	};
}

#endif //!GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H