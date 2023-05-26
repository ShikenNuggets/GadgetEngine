#ifndef GADGET_GUI_ELEMENT_H
#define GADGET_GUI_ELEMENT_H

#include "Input/InputEnums.h"
#include "Math/Vector.h"

namespace Gadget{
	enum class GuiAnchor{
		None,
		Center,
		Top,
		Bottom,
		Left,
		Right,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	class GuiElement{
	public:
		GuiElement(StringID name_, const Vector2& pos_, const Vector2& size_, GuiAnchor anchor_, bool isActive_ = true) : name(name_), position(pos_), size(size_), anchor(anchor_), isActive(isActive_){}
		
		virtual ~GuiElement(){
			for(const auto& se : subElements){
				delete se;
			}
		}

		virtual void Update([[maybe_unused]] float deltaTime_){}

		StringID GetName() const{ return name; }
		bool IsActive() const{ return isActive; }
		Vector2 GetPosition() const{ return position; }
		Vector2 GetSize() const{ return size; }
		GuiAnchor GetAnchor() const{ return anchor; }
		const std::vector<GuiElement*>& GetSubElements() const{ return subElements; }

		void AddSubElement(GuiElement* element){ subElements.push_back(element); }

		void SetName(StringID name_){ name = name_; }
		void SetIsActive(bool isActive_){ isActive = isActive_; }
		void SetPosition(const Vector2& pos_){ position = pos_; }
		void SetSize(const Vector2& size_){ size = size_; }
		void SetAnchor(GuiAnchor anchor_){ anchor = anchor_; }

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache the result when possible
		template <class T> T* GetSubElement() const{
			static_assert(std::is_base_of<GuiElement, T>::value, "T must inherit from GuiElement");
			T* element = nullptr;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(GuiElement* e : subElements){
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
		template <class T> std::vector<T*> GetSubElements() const{
			static_assert(std::is_base_of<GuiElement, T>::value, "T must inherit from GuiElement");
			std::vector<T*> ems;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(GuiElement* e : subElements){
				T* element = dynamic_cast<T*>(e);
				if(element != nullptr){
					ems.push_back(element);
				}

				auto se = e->GetSubElements<T>();
				ems.append_range(se);
			}

			return ems;
		}

		virtual void OnClick(ButtonID button_, const Vector2& clickPoint_){
			for(const auto& e : subElements){
				if(e->PointIntersects(clickPoint_)){
					e->OnClick(button_, clickPoint_);
				}
			}
		}

		bool PointIntersects(const Vector2& point_) const{
			return (point_.x >= position.x - size.x
				&& point_.x <= position.x + size.x
				&& point_.y >= position.y - size.y
				&& point_.y <= position.y + size.y);
		}

	private:
		StringID name;
		bool isActive;
		Vector2 position;
		Vector2 size;
		GuiAnchor anchor;
		std::vector<GuiElement*> subElements;
	};
}

#endif //!GADGET_GUI_ELEMENT_H