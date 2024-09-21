#include "ProjectBrowserWindow.h"

#include <imgui.h>

using namespace Gadget::Workbench;

ProjectBrowserWindow::ProjectBrowserWindow(const ProjectManager& projectManager_) : SubWindow("Project Browser"), allowInput(true), projectManager(projectManager_), onClickCreateProject(nullptr){}

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

		for(const auto& proj : projectManager.RecentProjects()){
			ImGui::Text(proj.GetName().Value());
			ImGui::Text(proj.GetPath().Value());
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