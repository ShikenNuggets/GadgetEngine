#include "CreateProjectWindow.h"

#include <imgui.h>

using namespace Gadget::Workbench;

CreateProjectWindow::CreateProjectWindow() : SubWindow("Create Project"), projectName(), projectPath(), onExit(nullptr), onClickCreateProject(nullptr){
	for(size_t i = 0; i < gNameCapacity; i++){
		projectName[i] = '\0';
	}

	for(size_t i = 0; i < gPathCapacity; i++){
		projectPath[i] = '\0';
	}
	
	GADGET_BASIC_ASSERT(!onExit.IsValid());
	GADGET_BASIC_ASSERT(!onClickCreateProject.IsValid());
}

void CreateProjectWindow::Draw(){
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 100.0f), ImVec2(8000.0f, 8000.0f));
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	bool keepOpen = true;
	if(ImGui::Begin("Create Project", &keepOpen, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)){
		ImGui::SetWindowFocus("Create Project");
		ImGui::Text("Project Name");
		ImGui::InputText("##ProjectName", projectName, gNameCapacity);
		ImGui::Text("Path");
		ImGui::InputText("##ProjectPath", projectPath, gPathCapacity);

		if(ImGui::Button("Create")){
			onClickCreateProject();
		}
	}

	ImGui::End();

	if(!keepOpen){
		onExit();
	}
}