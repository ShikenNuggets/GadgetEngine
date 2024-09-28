#include "EditorFlow.h"

#include "EditorApp.h"

using namespace Gadget;
using namespace Workbench;

EditorFlow::EditorFlow(){
	auto& wndManager = EditorApp::GetWndManager();
	auto& projManager = EditorApp::GetProjectManager();

	editorToolbar = wndManager.AddWindow<EditorToolbar>();
}

EditorFlow::~EditorFlow(){
	EditorApp::GetWndManager().RemoveWindow(editorToolbar);
}