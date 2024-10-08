#include "ProjectManager.h"

#include <imgui.h>

#include <Core/FileSystem.h>
#include <Serializers/ContainerSerializer.h>

#include "ProjectResources.h"
#include "Utils/VisualStudioHelper.h"

using namespace Gadget;
using namespace Workbench;

static constexpr const char* gRecentProjectsFilePath = "recent_projects.json";

ProjectManager::ProjectManager() : projects(), currentProject(nullptr), currentProjectResources(nullptr){
	LoadProjects();
	SaveProjects();
}

void ProjectManager::AddNewProject(const Project& project_){
	currentProject = &(projects.Add(project_));
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

			if(FileSystem::FileExists(p.FullProjectFilePath())){
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

	const std::string projectFilePath = project_.FullProjectFilePath();
	GADGET_BASIC_ASSERT(!FileSystem::FileExists(projectFilePath));
	if(FileSystem::FileExists(projectFilePath)){
		return ErrorCode::Invalid_Args;
	}

	nlohmann::json j;
	j["project_name"] = project_.GetName();
	j["vs_solution"] = project_.GetName() + VisualStudio::gVsSolutionExtension;
	j["resources_file"] = ResourceManager::gResourcesJsonFile;

	ErrorCode err = FileSystem::WriteJSONToPlainTextFile(projectFilePath, j);
	if(err != ErrorCode::OK){
		return err;
	}

	err = VisualStudio::GenerateSolution(project_.GetName(), project_.GetPath());
	if(err != ErrorCode::OK){
		return err;
	}

	currentProjectResources = new ProjectResources(project_);

	return FileSystem::WriteJSONToPlainTextFile(project_.GetPath() + ResourceManager::gResourcesJsonFile, {});
}