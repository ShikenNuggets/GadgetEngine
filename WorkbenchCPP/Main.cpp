#include "Core/EditorWindow.h"

using namespace Gadget::Workbench;

int main(int argc, char* argv[]){
	EditorWindow window = EditorWindow(1280, 960);

	bool shouldContinue = true;
	while(shouldContinue){
		shouldContinue = window.HandleEvents();
		window.Render();
	}

	return 0;
}