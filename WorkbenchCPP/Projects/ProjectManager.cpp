#include "ProjectManager.h"

#include <imgui.h>

#include <Core/FileSystem.h>
#include <Serializers/ContainerSerializer.h>

using namespace Gadget;
using namespace Workbench;

static constexpr const char* gRecentProjectsFilePath = "recent_projects.json";

ProjectManager::ProjectManager() : projects(){
	LoadProjects();
	SaveProjects();
}

void ProjectManager::AddNewProject(const Project& project_){
	projects.Add(project_);
	CreateNewProjectFile(project_);
	SaveProjects();
}

ErrorCode ProjectManager::LoadProjects(){
	if(!FileSystem::FileExists(gRecentProjectsFilePath)){
		return ErrorCode::OK;
	}

	auto json = FileSystem::ReadPlainTextJSONFile(gRecentProjectsFilePath);
	if(!json.is_null()){
		Array<Project> temp = json.get<Array<Project>>();
		for(const auto& p : temp){
			if(p.GetName().empty() || p.GetPath().empty()){
				GADGET_LOG_WARNING(SID("PROJ"), "Invalid data in recent projects file, ignoring...");
				continue;
			}

			if(FileSystem::FileExists(FullProjectFilePath(p.GetName(), p.GetPath()))){
				projects.Add(p);
			}else{
				GADGET_LOG(SID("PROJ"), "Project [" + p.GetName() + "] file not found, removing from recent projects list...");
			}
		}
	}

	return ErrorCode::OK;
}

ErrorCode ProjectManager::SaveProjects(){
	return FileSystem::WriteJSONToPlainTextFile(gRecentProjectsFilePath, projects);
}

ErrorCode ProjectManager::CreateNewProjectFile(const Project& project_){
	GADGET_BASIC_ASSERT(!project_.GetName().empty());
	GADGET_BASIC_ASSERT(!project_.GetPath().empty());

	const std::string projectFilePath = FullProjectFilePath(project_.GetName(), project_.GetPath());
	GADGET_BASIC_ASSERT(!FileSystem::FileExists(projectFilePath));
	if(FileSystem::FileExists(projectFilePath)){
		return ErrorCode::Invalid_Args;
	}

	return FileSystem::WriteJSONToPlainTextFile(projectFilePath, {});
}