#include "GuiCanvas.h"

#include "App.h"
#include "Utils/Utils.h"

using namespace Gadget;

GuiCanvas::GuiCanvas(StringID name_, bool isActive_) : name(name_), isActive(isActive_), elements(){}

GuiCanvas::~GuiCanvas(){
	for(const auto& e : elements){
		delete e;
	}
}

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

void GuiCanvas::OnMouseMoved([[maybe_unused]] int xPos, [[maybe_unused]] int yPos){
}

void GuiCanvas::OnMouseClick(ButtonID mouseButton){
	Vector2 clickPoint = Vector2(App::GetInput().GetCurrentMouseXInGUICoordinates(), App::GetInput().GetCurrentMouseYInGUICoordinates());

	switch(mouseButton){
		case Gadget::ButtonID::Mouse_LeftMouseButton:
		case Gadget::ButtonID::Mouse_RightMouseButton:

			for(const auto& e : elements){
				if(e->PointIntersects(clickPoint)){
					e->OnClick(mouseButton, clickPoint);
				}
			}

			break;
		default:
			break;
	}
}