#include "WindowManager.h"

using namespace Gadget::Workbench;

WindowManager::WindowManager() : windows(16){}

void WindowManager::AddWindow(SubWindow* window_){ windows.Add(window_); }

void WindowManager::RemoveWindow(SubWindow* window_){
	GADGET_BASIC_ASSERT(window_ != nullptr);
	GADGET_BASIC_ASSERT(windows.Contains(window_));
	removeList.Add(window_);
}

void WindowManager::Update(){
	for(auto* w : removeList){
		GADGET_BASIC_ASSERT(w != nullptr && w->value != nullptr);
		if(w == nullptr || w->value == nullptr){
			continue;
		}

		GADGET_BASIC_ASSERT(windows.Contains(w->value));
		windows.Remove(w->value);
		delete w->value;
		w->value = nullptr;
	}
	removeList.Clear();

	for(auto* w : windows){
		GADGET_BASIC_ASSERT(w != nullptr);
		if(w == nullptr){
			continue;
		}

		w->Draw();
	}
}