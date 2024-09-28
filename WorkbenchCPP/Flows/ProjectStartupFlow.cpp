#include "ProjectStartupFlow.h"

#include "EditorApp.h"
#include "Flows/EditorFlow.h"
#include "Windows/CreateProjectWindow.h"
#include "Windows/ProjectBrowserWindow.h"

using namespace Gadget;
using namespace Workbench;

ProjectStartupFlow::ProjectStartupFlow() : Flow(), createProjectWindow(nullptr), projectBrowserWindow(nullptr){
	auto& wndManager = EditorApp::GetWndManager();
	auto& projManager = EditorApp::GetProjectManager();

	projectBrowserWindow = wndManager.AddWindow<ProjectBrowserWindow>(projManager);
	projectBrowserWindow->OnClickCreateProject([&](){
		createProjectWindow = wndManager.AddWindow<CreateProjectWindow>();
		GADGET_BASIC_ASSERT(createProjectWindow != nullptr);
		createProjectWindow->OnExit([&](){
			EditorApp::GetWndManager().RemoveWindow(createProjectWindow);
			projectBrowserWindow->EnableInput();
		});

		createProjectWindow->OnClickCreateProject([&](){
			EditorApp::GetProjectManager().AddNewProject(Project(createProjectWindow->GetNameField(), createProjectWindow->GetPathField()));
			isFlowComplete = true;
		});
	});

	projectBrowserWindow->OnClickOpenProject([&](){
		isFlowComplete = true;
	});
}

ProjectStartupFlow::~ProjectStartupFlow(){
	if(createProjectWindow != nullptr){
		EditorApp::GetWndManager().RemoveWindow(createProjectWindow);
	}

	GADGET_BASIC_ASSERT(projectBrowserWindow != nullptr);
	if(projectBrowserWindow != nullptr){
		EditorApp::GetWndManager().RemoveWindow(projectBrowserWindow);
	}
}

Flow* ProjectStartupFlow::NextFlow(){
	return new EditorFlow();
}