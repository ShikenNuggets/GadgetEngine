#ifndef GADGET_WORKBENCH_WINDOWS_CREATE_PROJECT_WINDOW_H
#define GADGET_WORKBENCH_WINDOWS_CREATE_PROJECT_WINDOW_H

#include "Windows/SubWindow.h"

namespace Gadget::Workbench{
	class CreateProjectWindow : public SubWindow{
	public:
		CreateProjectWindow();

		virtual void Draw() override;

	private:
		std::string projectName;
		std::string projectPath;
	};
}

#endif //!GADGET_WORKBENCH_WINDOWS_CREATE_PROJECT_WINDOW_H