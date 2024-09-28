#ifndef GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H
#define GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H

#include <Data/String.h>

#include "Project.h"

namespace Gadget::Workbench{
	class ProjectManager{
	public:
		ProjectManager();

		bool HasRecentProjects() const{ return projects.IsEmpty(); }
		const Array<Project>& RecentProjects() const{ return projects; }

		void AddNewProject(const Project& project_);

	private:
		Array<Project> projects;

		static constexpr inline const char* gProjectExtension = ".wbn";

		ErrorCode LoadProjects();
		ErrorCode SaveProjects();

		ErrorCode CreateNewProjectFile(const Project& project_);

		static constexpr inline std::string FullProjectFilePath(const std::string& name_, const std::string& path_){
			return path_ + name_ + gProjectExtension;
		}
	};
}

#endif //!GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H