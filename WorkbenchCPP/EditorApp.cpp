#include "EditorApp.h"

#include "Windows/CreateProjectWindow.h"
#include "Windows/ProjectBrowserWindow.h"

using namespace Gadget;
using namespace Workbench;

std::unique_ptr<EditorApp> EditorApp::instance;

EditorApp::EditorApp() : isRunning(true), window(1280, 960), projManager(), wndManager(), createProjectWindowPtr(nullptr), projectBrowserWindowPtr(nullptr){
	//TODO - This is getting pretty gross, I think there's a better way
	projectBrowserWindowPtr = wndManager.AddWindow<ProjectBrowserWindow>(projManager);
	projectBrowserWindowPtr->OnClickCreateProject([&](){
		createProjectWindowPtr = wndManager.AddWindow<CreateProjectWindow>();
		GADGET_BASIC_ASSERT(createProjectWindowPtr != nullptr);
		createProjectWindowPtr->OnExit([&](){
			EditorApp::GetWndManager().RemoveWindow(createProjectWindowPtr);
			projectBrowserWindowPtr->EnableInput();
		});

		createProjectWindowPtr->OnClickCreateProject([&](){
			EditorApp::GetProjectManager().AddNewProject(Project(createProjectWindowPtr->GetNameField(), createProjectWindowPtr->GetPathField()));

			EditorApp::GetWndManager().RemoveWindow(createProjectWindowPtr);
			EditorApp::GetWndManager().RemoveWindow(projectBrowserWindowPtr);
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