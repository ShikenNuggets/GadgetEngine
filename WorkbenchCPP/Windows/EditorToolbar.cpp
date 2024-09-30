#include "EditorToolbar.h"

#include <imgui.h>

using namespace Gadget;
using namespace Workbench;

void EditorToolbar::Draw(){
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	
	if(ImGui::Begin("MainToolbar", nullptr, flags)){
		if(ImGui::BeginMenuBar()){
			if(ImGui::MenuItem("Scenes")){
				currentTab = Tab::Scenes;
			}

			if(ImGui::MenuItem("Objects")){
				currentTab = Tab::Objects;
			}

			if(ImGui::MenuItem("Resources")){
				currentTab = Tab::Resources;
			}

			ImGui::EndMenuBar();
		}
		
		GADGET_BASIC_ASSERT(currentTab != Tab::Tab_MAX);
		switch(currentTab){
			case Tab::Scenes:
				break;
			case Tab::Objects:
				break;
			case Tab::Resources:
				break;
			default:
				break;
		}
	}

	ImGui::End();
}