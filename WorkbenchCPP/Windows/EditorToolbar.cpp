#include "EditorToolbar.h"

#include <imgui.h>

#include "Windows/Tabs/ResourcesTab.h"

using namespace Gadget;
using namespace Workbench;

EditorToolbar::EditorToolbar() : SubWindow("Toolbar"), currentTab(Tab::None){
	tabs[static_cast<int64_t>(Tab::None)] = new SubTab();
	tabs[static_cast<int64_t>(Tab::Scenes)] = new SubTab();
	tabs[static_cast<int64_t>(Tab::Objects)] = new SubTab();
	tabs[static_cast<int64_t>(Tab::Resources)] = new ResourcesTab();
}

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

		GADGET_BASIC_ASSERT(static_cast<int64_t>(currentTab) >= 0);
		GADGET_BASIC_ASSERT(static_cast<int64_t>(currentTab) < tabs.Size());
		GADGET_BASIC_ASSERT(currentTab < Tab::Tab_MAX);

		SubTab* tab = tabs[static_cast<int64_t>(currentTab)];
		GADGET_BASIC_ASSERT(tab != nullptr);
		if(tab != nullptr){
			tabs[static_cast<int64_t>(currentTab)]->Update();
			tabs[static_cast<int64_t>(currentTab)]->Draw();
		}
	}

	ImGui::End();
}