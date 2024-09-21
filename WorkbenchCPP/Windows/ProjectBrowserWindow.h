#ifndef GADGET_WORKBENCH_PROJECT_BROWSER_WINDOW_H
#define GADGET_WORKBENCH_PROJECT_BROWSER_WINDOW_H

#include "Projects/ProjectManager.h"
#include "Windows/SubWindow.h"

namespace Gadget::Workbench{
	class ProjectBrowserWindow : public SubWindow{
	public:
		ProjectBrowserWindow(const ProjectManager& projectManager_);

		void EnableInput(){ allowInput = true; }
		void DisableInput(){ allowInput = false; }

		virtual void Draw() override;

	private:
		bool allowInput;
		const ProjectManager& projectManager;
	};
}

#endif //!GADGET_WORKBENCH_PROJECT_BROWSER_WINDOW_H