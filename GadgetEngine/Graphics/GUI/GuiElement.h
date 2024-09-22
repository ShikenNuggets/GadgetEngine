#ifndef GADGET_GUI_ELEMENT_H
#define GADGET_GUI_ELEMENT_H

#include "Data/Array.h"
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
		BottomRight,

		GuiAnchor_MAX //Don't put any values under this!
	};

	class GuiElement{
	public:
		GuiElement(StringID name_, const Vector2& pos_, const Vector2& size_, GuiAnchor anchor_, bool isActive_ = true) : name(name_), isActive(isActive_), position(pos_), size(size_), anchor(anchor_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(pos_.IsValid());
			GADGET_BASIC_ASSERT(size_.IsValid());
			GADGET_BASIC_ASSERT(anchor_ != GuiAnchor::GuiAnchor_MAX);
		}
		
		virtual ~GuiElement(){
			for(const auto& se : subElements){
				delete se;
			}
		}

		virtual void Update([[maybe_unused]] float deltaTime_){ GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_) && deltaTime_ >= 0.0f); }

		StringID GetName() const{ return name; }
		bool IsActive() const{ return isActive; }
		Vector2 GetPosition() const{ return position; }
		Vector2 GetSize() const{ return size; }
		GuiAnchor GetAnchor() const{ return anchor; }

		void AddSubElement(GuiElement* element_){
			GADGET_BASIC_ASSERT(element_ != nullptr);
			subElements.push_back(element_);
		}

		void SetName(StringID name_){ name = name_; }

		void SetIsActive(bool isActive_){
			isActive = isActive_;
			for(auto* se : subElements){
				GADGET_BASIC_ASSERT(se != nullptr);
				se->SetIsActive(isActive_);
			}
		}

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
		template <class T> Array<T*> GetSubElements() const{
			static_assert(std::is_base_of<GuiElement, T>::value, "T must inherit from GuiElement");
			Array<T*> ems;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(GuiElement* e : subElements){
				GADGET_BASIC_ASSERT(e != nullptr);
				if(e == nullptr){
					continue;
				}

				T* element = dynamic_cast<T*>(e);
				if(element != nullptr){
					ems.Add(element);
				}

				//auto se = e->GetSubElements<T>();
				//ems.append_range(se); //Cool feature, doesn't exist yet as of C++20
				//ems.reserve(ems.size() + se.size());
				//ems.insert(ems.end(), se.begin(), se.end());
				ems.Add(e->GetSubElements<T>());
			}

			return ems;
		}

		template <class T> void GetSubElements(Array<T*>& inBuffer_) const{
			inBuffer_.Reserve(inBuffer_.Size() + subElements.size());
			for(const auto& e : subElements){
				T* element = dynamic_cast<T*>(e);
				if(element != nullptr){
					inBuffer_.Add(element);
				}
			}
		}

		virtual void OnClick(ButtonID button_, const Vector2& clickPoint_){
			GADGET_BASIC_ASSERT(button_ != ButtonID::ButtonID_MAX);
			GADGET_BASIC_ASSERT(clickPoint_.IsValid());

			for(const auto& e : subElements){
				if(e->PointIntersects(clickPoint_)){
					e->OnClick(button_, clickPoint_);
				}
			}
		}

		bool PointIntersects(const Vector2& point_) const{
			GADGET_BASIC_ASSERT(point_.IsValid());

			return (point_.x >= position.x - size.x
				&& point_.x <= position.x + size.x
				&& point_.y >= position.y - size.y
				&& point_.y <= position.y + size.y);
		}

	protected:
		StringID name;
		bool isActive;
		Vector2 position;
		Vector2 size;
		GuiAnchor anchor;
		std::vector<GuiElement*> subElements;
	};
}

#endif //!GADGET_GUI_ELEMENT_H