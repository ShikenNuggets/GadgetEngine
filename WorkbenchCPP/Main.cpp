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
	wndManager.AddWindow(new ProjectBrowserWindow(projManager));

	while(true){
		if(!window.HandleEvents()){
			break;
		}

		wndManager.Update();
		window.Render();
	}

	return 0;
}