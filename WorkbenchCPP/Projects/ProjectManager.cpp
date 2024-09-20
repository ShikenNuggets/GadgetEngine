#include "ProjectManager.h"

#include <imgui.h>

#include <Core/FileSystem.h>

using namespace Gadget::Workbench;

static constexpr const char* gRecentProjectsFilePath = "recent_projects.json";

ProjectManager::ProjectManager() : projects(), guiShowCreateProjectWindow(false), guiProjectName(), guiProjectPath(){
	guiProjectName.reserve(64);
	guiProjectPath.reserve(128);

	if(FileSystem::FileExists(gRecentProjectsFilePath)){
		auto json = FileSystem::ReadPlainTextJSONFile(gRecentProjectsFilePath);

		if(!json.is_null()){
			for(auto& element : json){
				const String name = static_cast<std::string>(element.at("name"));
				const String path = static_cast<std::string>(element.at("path"));

				projects.Emplace(name, path);
			}
		}
	}

	//TODO
	//ErrorCode err = FileSystem::WriteJSONToPlainTextFile(gRecentProjectsFilePath, {});
	//if(err != ErrorCode::OK){
	//	Debug::ThrowFatalError(SID("PROJECT"), "ProjectManager could not write recent projects file!", err, __FILE__, __LINE__);
	//}
}

void ProjectManager::Draw(){
	//------------------------------//
	//------- Project Browser ------//
	//------------------------------//
	if(ImGui::Begin("Project Browser")){
		ImGui::Text("Choose a project:");
		ImGui::Spacing();

		for(const auto& proj : projects){
			ImGui::Text(proj.GetName().Value());
			ImGui::Text(proj.GetPath().Value());
			ImGui::Spacing();
		}

		if(ImGui::Button("Create New Project")){
			guiShowCreateProjectWindow = true;
		}
	}
	
	ImGui::End();

	//------------------------------//
	//------- Create Project -------//
	//------------------------------//
	if(guiShowCreateProjectWindow){
		if(ImGui::Begin("Create Project", &guiShowCreateProjectWindow, ImGuiWindowFlags_NoSavedSettings)){
			ImGui::Text("Project Name");
			ImGui::InputText("##ProjectName", guiProjectName.data(), guiProjectName.capacity());
			ImGui::Text("Path");
			ImGui::InputText("##ProjectPath", guiProjectPath.data(), guiProjectPath.capacity());

			if(ImGui::Button("Create")){
				//TODO
				guiShowCreateProjectWindow = false;
			}
		}

		ImGui::End();
	}
}