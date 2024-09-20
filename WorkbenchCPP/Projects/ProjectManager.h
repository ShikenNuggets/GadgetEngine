#ifndef GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H
#define GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H

#include <Data/String.h>

#include "Project.h"

namespace Gadget::Workbench{
	class ProjectManager{
	public:
		ProjectManager();

		void Draw();

		bool HasRecentProjects() const{ return projects.IsEmpty(); }
		const Array<Project>& RecentProjects() const{ return projects; }

	private:
		Array<Project> projects;

		bool guiShowCreateProjectWindow;
		std::string guiProjectName;
		std::string guiProjectPath;
	};
}

#endif //!GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H