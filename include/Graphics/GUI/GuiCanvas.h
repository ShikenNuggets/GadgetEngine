#ifndef GADGET_GUI_CANVAS_H
#define GADGET_GUI_CANVAS_H

#include <vector>

#include "Data/Array.h"
#include "Graphics/GUI/GuiElement.h"
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
		template <class T> Array<T*> GetElements() const{
			static_assert(std::is_base_of<GuiElement, T>::value, "T must inherit from GuiElement");
			Array<T*> ems;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(GuiElement* e : elements){
				GADGET_BASIC_ASSERT(e != nullptr);
				T* element = dynamic_cast<T*>(e);
				if(element != nullptr){
					ems.Add(element);
				}

				auto se = e->GetSubElements<T>();
				//ems.append_range(se); //Cool feature, doesn't exist yet as of C++20
				//ems.Reserve(ems.size() + se.size());
				//ems.Insert(ems.end(), se.begin(), se.end());
				ems.Add(se);
			}

			return ems;
		}

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache the result when possible
		template <class T> void GetElements(Array<T*>& inBuffer_){
			GADGET_ASSERT(inBuffer_.IsEmpty(), "Non-empty Array passed to GetElements, existing data will be lost!");
			inBuffer_.Clear();
			inBuffer_.Reserve(elements.size());

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(GuiElement* e : elements){
				GADGET_BASIC_ASSERT(e != nullptr);
				T* element = dynamic_cast<T*>(e);
				if(element != nullptr){
					inBuffer_.Add(element);
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