#include "Core/EditorWindow.h"
#include "Projects/ProjectManager.h"

using namespace Gadget::Workbench;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){
	EditorWindow window = EditorWindow(1280, 960);

	ProjectManager manager = ProjectManager();

	while(true){
		if(!window.HandleEvents()){
			break;
		}

		manager.Draw();

		window.Render();
	}

	return 0;
}