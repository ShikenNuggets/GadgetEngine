#include "Core/EditorWindow.h"
#include "Projects/ProjectManager.h"

using namespace Gadget::Workbench;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){
	EditorWindow window = EditorWindow(1280, 960);

	ProjectManager manager = ProjectManager();

	bool shouldContinue = true;
	while(shouldContinue){
		shouldContinue = window.HandleEvents();
		window.Render();
	}

	return 0;
}