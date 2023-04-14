#ifndef GADGET_GUI_ELEMENT_H
#define GADGET_GUI_ELEMENT_H

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

		virtual void Update([[maybe_unused]] float deltaTime_){}

		StringID GetName() const{ return name; }
		bool IsActive() const{ return isActive; }
		Vector2 GetPosition() const{ return position; }
		Vector2 GetSize() const{ return size; }
		GuiAnchor GetAnchor() const{ return anchor; }

		void SetName(StringID name_){ name = name_; }
		void SetIsActive(bool isActive_){ isActive = isActive_; }
		void SetPosition(const Vector2& pos_){ position = pos_; }
		void SetSize(const Vector2& size_){ size = size_; }
		void SetAnchor(GuiAnchor anchor_){ anchor = anchor_; }

	private:
		StringID name;
		bool isActive;
		Vector2 position;
		Vector2 size;
		GuiAnchor anchor;
	};
}

#endif //!GADGET_GUI_ELEMENT_H