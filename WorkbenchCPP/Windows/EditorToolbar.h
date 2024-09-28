#ifndef GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H
#define GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H

#include "Windows/SubWindow.h"

namespace Gadget::Workbench{
	class EditorToolbar : public SubWindow{
	public:
		EditorToolbar() : SubWindow("Toolbar"){}

		virtual void Draw() override;
	};
}

#endif //!GADGET_WORKBENCH_WINDOWS_EDITOR_TOOLBAR_H