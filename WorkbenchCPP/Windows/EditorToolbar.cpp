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
			ImGui::MenuItem("Scenes");
			ImGui::MenuItem("Objects");
			ImGui::MenuItem("Resources");

			ImGui::EndMenuBar();
		}
	}

	ImGui::End();
}