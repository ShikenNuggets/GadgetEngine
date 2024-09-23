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

ErrorCode ProjectManager::LoadProjects(){
	if(!FileSystem::FileExists(gRecentProjectsFilePath)){
		return ErrorCode::OK;
	}

	auto json = FileSystem::ReadPlainTextJSONFile(gRecentProjectsFilePath);
	if(!json.is_null()){
		Array<Project> temp = json.get<Array<Project>>();
		for(const auto& p : temp){
			if(FileSystem::FileExists(p.GetPath())){
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