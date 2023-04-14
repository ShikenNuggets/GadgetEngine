#include "GuiCanvas.h"

#include "Utils/Utils.h"

using namespace Gadget;

GuiCanvas::GuiCanvas(StringID name_, bool isActive_) : name(name_), isActive(isActive_), elements(){}

void GuiCanvas::AddElement(GuiElement* element_){
	GADGET_BASIC_ASSERT(element_ != nullptr);
	GADGET_ASSERT(!Utils::Contains(elements, element_), "Tried to add an element to this canvas multiple times!");
	elements.push_back(element_);
}

void GuiCanvas::Update(float deltaTime_){
	for(auto& e : elements){
		GADGET_BASIC_ASSERT(e != nullptr);
		e->Update(deltaTime_);
	}
}

GuiElement* GuiCanvas::GetElement(StringID name_){
	for(const auto& e : elements){
		GADGET_BASIC_ASSERT(e != nullptr);
		if(e->GetName() == name_){
			return e;
		}
	}

	Debug::Log("Tried to get a GuiElement [" + name.GetString() + "], but it could not be found!", Debug::Warning, __FILE__, __LINE__);
	return nullptr;
}