#ifndef GADGET_GUI_CANVAS_H
#define GADGET_GUI_CANVAS_H

#include <vector>

#include "GuiElement.h"

namespace Gadget{
	class GuiCanvas{
	public:
		GuiCanvas(StringID name_, bool isActive_ = true);

		void AddElement(GuiElement* element_);

		virtual void Update(float deltaTime_);

	private:
		StringID name;
		bool isActive;
		std::vector<GuiElement*> elements;
	};
}

#endif //!GADGET_GUI_CANVAS_H