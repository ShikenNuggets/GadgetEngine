#include "EditorApp.h"

#include "Flows/ProjectStartupFlow.h"

using namespace Gadget;
using namespace Workbench;

std::unique_ptr<EditorApp> EditorApp::instance;

EditorApp::EditorApp() : isRunning(true), window(1280, 960), projManager(), wndManager(), fontManager(), currentFlow(nullptr){}

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
	currentFlow = new ProjectStartupFlow();

	isRunning = true;
	while(isRunning){
		if(currentFlow != nullptr && currentFlow->IsFlowComplete()){
			Flow* nextFlow = currentFlow->NextFlow();
			delete currentFlow;
			currentFlow = nextFlow;
		}
		
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