#ifndef GADGET_WORKBENCH_PROJECTS_PROJECT_RESOURCES_H
#define GADGET_WORKBENCH_PROJECTS_PROJECT_RESOURCES_H

#include <Data/HashTable.h>

#include "Project.h"

namespace Gadget::Workbench{
	class ProjectResources{
	public:
		ProjectResources(const Project& project_);

		void AddResource(const StringID& name_);
		void SetVar(const StringID& name_, const StringID& varName_, const Var& value_);

		void Load();
		ErrorCode Save();

	private:
		HashTable<StringID, NamedVarList> resources;
		const Project& project;
	};
}

#endif //!GADGET_WORKBENCH_PROJECTS_PROJECT_RESOURCES_H