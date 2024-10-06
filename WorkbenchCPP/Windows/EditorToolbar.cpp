#include "EditorToolbar.h"

#include <imgui.h>

#include <Platform/Windows/Win32_Utils.h>

#include "EditorApp.h"
#include "Utils/VisualStudioHelper.h"
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

			if(ImGui::MenuItem("Code")){
				auto* currentProject = EditorApp::CurrentProject();
				GADGET_BASIC_ASSERT(currentProject != nullptr);
				if(currentProject == nullptr){
					return;
				}

				const ErrorCode err = VisualStudio::OpenSolution(currentProject->GetName(), currentProject->GetPath());
				switch(err){
					case ErrorCode::OK: break;
					case ErrorCode::Win32_FileIO_PermissionsError:
						Debug::PopupErrorMessage("File Permissions Error", "Workbench does not have permission to open the Visual Studio solution. Try running Workbench as administrator.");
						break;
					case ErrorCode::Win32_NoAssociationError:
						Debug::PopupErrorMessage("No Associated Program Error", "You do not have a default program set in Windows for opening Visual Studio solutions.");
						break;
					case ErrorCode::Invalid_Args:
						Debug::PopupErrorMessage("Error", "Could not focus the existing Visual Studio window. If the issue persists, please contact the developer of Gadget Engine/Workbench. Error Code: " + GetErrorCodeString(err));
						break;
					default:
						Debug::PopupErrorMessage("Error", "Could not open Visual Studio. If the issue persists, please contact the developer of Gadget Engine/Workbench. Error Code: " + GetErrorCodeString(err));
						break;
				}
			}

			if(ImGui::MenuItem("Play")){
				//TODO - Launch game
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