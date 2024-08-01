#ifndef GADGET_GUI_CANVAS_H
#define GADGET_GUI_CANVAS_H

#include <vector>

#include "GuiElement.h"
#include "Input/InputEnums.h"

namespace Gadget{
	class GuiCanvas{
	public:
		GuiCanvas(StringID name_, bool isActive_ = true);
		virtual ~GuiCanvas();

		void AddElement(GuiElement* element_);

		virtual void Update(float deltaTime_);

		GuiElement* GetElement(StringID name_);

		virtual void OnMouseMoved(int xPos, int yPos);
		virtual void OnMouseClick(ButtonID mouseButton);

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache the result when possible
		template <class T> T* GetElement() const{
			static_assert(std::is_base_of<GuiElement, T>::value, "T must inherit from GuiElement");
			T* element = nullptr;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(GuiElement* e : elements){
				GADGET_BASIC_ASSERT(e != nullptr);
				element = dynamic_cast<T*>(e);
				if(element != nullptr){
					return element;
				}

				auto* se = e->GetSubElement<T>();
				if(se != nullptr){
					return se;
				}
			}

			return nullptr;
		}

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache the result when possible
		template <class T> std::vector<T*> GetElements() const{
			static_assert(std::is_base_of<GuiElement, T>::value, "T must inherit from GuiElement");
			std::vector<T*> ems;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(GuiElement* e : elements){
				GADGET_BASIC_ASSERT(e != nullptr);
				T* element = dynamic_cast<T*>(e);
				if(element != nullptr){
					ems.push_back(element);
				}

				auto se = e->GetSubElements<T>();
				//ems.append_range(se); //Cool feature, doesn't exist yet as of C++20
				ems.reserve(ems.size() + se.size());
				ems.insert(ems.end(), se.begin(), se.end());
			}

			return ems;
		}

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache the result when possible
		template <class T> void GetElements(std::vector<T*>& inBuffer_){
			GADGET_ASSERT(inBuffer_.empty(), "Non-empty std::vector passed to GetElements, existing data will be lost!");
			inBuffer_.clear();
			inBuffer_.reserve(elements.size());

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(GuiElement* e : elements){
				GADGET_BASIC_ASSERT(e != nullptr);
				T* element = dynamic_cast<T*>(e);
				if(element != nullptr){
					inBuffer_.push_back(element);
				}

				e->GetSubElements<T>(inBuffer_);
			}
		}

	private:
		StringID name;
		bool isActive;
		std::vector<GuiElement*> elements;
	};
}

#endif //!GADGET_GUI_CANVAS_H