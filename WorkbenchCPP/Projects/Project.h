#ifndef GADGET_WORKBENCH_PROJECTS_PROJECT_H
#define GADGET_WORKBENCH_PROJECTS_PROJECT_H

#include <chrono>

#include <Data/String.h>

namespace Gadget::Workbench{
	class Project{
	public:
		Project(const String name_, const String path_) : name(name_), path(path_){}

		const String& GetName() const{ return name; }
		const String& GetPath() const{ return path; }

	private:
		String name;
		String path;
	};
}

#endif //!GADGET_WORKBENCH_PROJECTS_PROJECT_H