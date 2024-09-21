#include "WindowManager.h"

using namespace Gadget::Workbench;

WindowManager::WindowManager() : windows(16){}

void WindowManager::AddWindow(SubWindow* window_){ windows.Add(window_); }

void WindowManager::Update(){
	for(auto* w : windows){
		GADGET_BASIC_ASSERT(w != nullptr);
		if(w == nullptr){
			continue;
		}

		w->Draw();
	}
}