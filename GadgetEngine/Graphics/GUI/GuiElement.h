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
		GuiElement(const Vector2& pos_, GuiAnchor anchor_, bool isActive_ = true) : position(pos_), anchor(anchor_), isActive(isActive_){}

		virtual void Update([[maybe_unused]] float deltaTime_){}

		Vector2 GetPosition() const{ return position; }
		GuiAnchor GetAnchor() const{ return anchor; }

		void GetPosition(const Vector2& pos_){ position = pos_; }
		void SetAnchor(GuiAnchor anchor_){ anchor = anchor_; }

	private:
		bool isActive;
		Vector2 position;
		GuiAnchor anchor;
	};
}

#endif //!GADGET_GUI_ELEMENT_H