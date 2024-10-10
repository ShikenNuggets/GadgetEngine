#ifndef GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H
#define GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H

#include <Data/String.h>

#include "Project.h"
#include "ProjectResources.h"

namespace Gadget::Workbench{
	class ProjectManager{
	public:
		ProjectManager();

		bool HasRecentProjects() const{ return projects.IsEmpty(); }
		const Array<Project>& RecentProjects() const{ return projects; }
		Array<Project>& RecentProjects(){ return projects; }

		void AddNewProject(const Project& project_);

		void SetCurrentProject(Project* project_){
			currentProject = project_;
			delete currentProjectResources;
			currentProjectResources = nullptr;

			if(currentProject != nullptr){
				currentProjectResources = new ProjectResources(*currentProject);
			}
		}

		Project* CurrentProject() const{ return currentProject; }
		ProjectResources* GetResources(){ return currentProjectResources; }

	private:
		Array<Project> projects;
		Project* currentProject;
		ProjectResources* currentProjectResources;

		ErrorCode LoadProjects();
		ErrorCode SaveProjects();

		ErrorCode CreateNewProjectFile(const Project& project_);
	};
}

#endif //!GADGET_WORKBENCH_PROJECTS_PROJECT_MANAGER_H