#include "Core/EditorWindow.h"
#include "Core/WindowManager.h"
#include "Projects/ProjectManager.h"
#include "Windows/CreateProjectWindow.h"
#include "Windows/ProjectBrowserWindow.h"

using namespace Gadget::Workbench;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){
	EditorWindow window = EditorWindow(1280, 960);
	ProjectManager projManager;
	WindowManager wndManager;

	auto* projBrowser = wndManager.AddWindow<ProjectBrowserWindow>(projManager);
	projBrowser->OnClickCreateProject([&](){
		auto* createProjWindow = wndManager.AddWindow<CreateProjectWindow>();
		createProjWindow->OnExit([&wndManager, createProjWindow, projBrowser](){
			wndManager.RemoveWindow(createProjWindow);
			projBrowser->EnableInput();
		});

		createProjWindow->OnClickCreateProject([&projManager, &wndManager, projBrowser, createProjWindow](){
			wndManager.RemoveWindow(createProjWindow);
			wndManager.RemoveWindow(projBrowser);
		});
	});

	while(true){
		if(!window.HandleEvents()){
			break;
		}

		wndManager.Update();
		window.Render();
	}

	return 0;
}