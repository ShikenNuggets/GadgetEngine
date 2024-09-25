#include "EditorApp.h"

#include "Windows/CreateProjectWindow.h"
#include "Windows/ProjectBrowserWindow.h"

using namespace Gadget;
using namespace Workbench;

std::unique_ptr<EditorApp> EditorApp::instance;

EditorApp::EditorApp() : isRunning(true), window(1280, 960), projManager(), wndManager(){
	auto* projBrowser = wndManager.AddWindow<ProjectBrowserWindow>(projManager);
	projBrowser->OnClickCreateProject([&](){
		auto* createProjWindow = wndManager.AddWindow<CreateProjectWindow>();
		GADGET_BASIC_ASSERT(createProjWindow != nullptr);
		createProjWindow->OnExit([createProjWindow, projBrowser](){
			EditorApp::GetWndManager().RemoveWindow(createProjWindow);
			projBrowser->EnableInput();
		});

		createProjWindow->OnClickCreateProject([projBrowser, createProjWindow](){
			EditorApp::GetProjectManager().AddNewProject(Project(createProjWindow->GetNameField(), createProjWindow->GetPathField()));

			EditorApp::GetWndManager().RemoveWindow(createProjWindow);
			EditorApp::GetWndManager().RemoveWindow(projBrowser);
		});
	});
}

EditorApp& EditorApp::GetInstance(){
	if(instance == nullptr){
		instance = std::make_unique<EditorApp>();
	}

	GADGET_ASSERT(instance != nullptr, "EditorApp instance was somehow nullptr! Nothing will work!");
	return *instance;
}

void EditorApp::DeleteInstance(){
	if(instance != nullptr){
		instance.reset();
	}
}

void EditorApp::Run(){
	isRunning = true;
	while(isRunning){
		if(!window.HandleEvents()){
			break;
		}

		wndManager.Update();
		window.Render();
	}
}

void EditorApp::Close(){
	GetInstance().isRunning = false;
}

uint64_t EditorApp::GetMainWindowHandle(){
	return GetInstance().window.GetWindowHandle();
}