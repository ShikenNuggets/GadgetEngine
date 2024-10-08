#include "ProjectBrowserWindow.h"

#include <imgui.h>

using namespace Gadget::Workbench;

ProjectBrowserWindow::ProjectBrowserWindow(ProjectManager& projectManager_) : SubWindow("Project Browser"), allowInput(true), projectManager(projectManager_), onClickCreateProject(nullptr), onClickOpenProject(nullptr){}

void ProjectBrowserWindow::Draw(){
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	if(!allowInput){
		flags |= ImGuiWindowFlags_NoInputs;
	}

	if(ImGui::Begin("Project Browser", nullptr, flags)){
		ImGui::Text("Choose a project:");
		ImGui::Spacing();

		for(auto& proj : projectManager.RecentProjects()){
			ImGui::Text(proj.GetName().c_str());
			ImGui::Text(proj.GetPath().c_str());
			if(ImGui::Button(("Load " + proj.GetName()).c_str())){
				projectManager.SetCurrentProject(&proj);
				onClickOpenProject();
			}
			ImGui::Spacing();
		}

		if(ImGui::Button("Create New Project")){
			GADGET_BASIC_ASSERT(onClickCreateProject.IsValid());
			DisableInput();
			onClickCreateProject();
		}
	}

	ImGui::End();
}