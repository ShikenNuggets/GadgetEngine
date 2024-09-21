#include "CreateProjectWindow.h"

#include <imgui.h>

using namespace Gadget::Workbench;

CreateProjectWindow::CreateProjectWindow() : SubWindow("Create Project"){}

void CreateProjectWindow::Draw(){
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 100.0f), ImVec2(8000.0f, 8000.0f));
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if(ImGui::Begin("Create Project", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)){
		ImGui::SetWindowFocus("Create Project");
		ImGui::Text("Project Name");
		ImGui::InputText("##ProjectName", projectName.data(), projectName.capacity());
		ImGui::Text("Path");
		ImGui::InputText("##ProjectPath", projectPath.data(), projectPath.capacity());

		if(ImGui::Button("Create")){
			//TODO
		}
	}

	ImGui::End();
}