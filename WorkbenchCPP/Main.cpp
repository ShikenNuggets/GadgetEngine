#include "Core/EditorWindow.h"
#include "Core/WindowManager.h"
#include "Projects/ProjectManager.h"
#include "Windows/CreateProjectWindow.h"
#include "Windows/ProjectBrowserWindow.h"

#include "EditorApp.h"

using namespace Gadget::Workbench;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){
	EditorApp::GetInstance().Run();
	return 0;
}