#ifndef GADGET_WORKBENCH_FLOWS_PROJECT_STARTUP_FLOW_H
#define GADGET_WORKBENCH_FLOWS_PROJECT_STARTUP_FLOW_H

#include "Core/EditorWindow.h"
#include "Core/WindowManager.h"
#include "Flows/Flow.h"

namespace Gadget::Workbench{
	class CreateProjectWindow;
	class ProjectBrowserWindow;

	class ProjectStartupFlow : public Flow{
	public:
		ProjectStartupFlow();
		virtual ~ProjectStartupFlow() override;

		virtual Flow* NextFlow() override;
		
	private:
		CreateProjectWindow* createProjectWindow;
		ProjectBrowserWindow* projectBrowserWindow;
	};
}

#endif //!GADGET_WORKBENCH_FLOWS_PROJECT_STARTUP_FLOW_H