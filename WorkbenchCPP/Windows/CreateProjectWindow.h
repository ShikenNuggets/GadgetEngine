#ifndef GADGET_WORKBENCH_WINDOWS_CREATE_PROJECT_WINDOW_H
#define GADGET_WORKBENCH_WINDOWS_CREATE_PROJECT_WINDOW_H

#include "Core/Callback.h"
#include "Windows/SubWindow.h"

namespace Gadget::Workbench{
	class CreateProjectWindow : public SubWindow{
	public:
		CreateProjectWindow();

		virtual void Draw() override;

		void OnExit(Callback func_){
			GADGET_BASIC_ASSERT(func_.IsValid());
			GADGET_BASIC_ASSERT(!onExit.IsValid());
			if(onExit.IsValid()){
				GADGET_LOG_WARNING(SID("PROJ"), "CreateProjectWindow::OnExit handler was set twice. Original value will be overwritten. Are you sure this is what you wanted?");
			}

			onExit = func_;
		}

		void OnClickCreateProject(Callback func_){
			GADGET_BASIC_ASSERT(func_.IsValid());
			GADGET_BASIC_ASSERT(!onClickCreateProject.IsValid());
			if(onClickCreateProject.IsValid()){
				GADGET_LOG_WARNING(SID("PROJ"), "CreateProjectWindow::OnClickCreateProject handler was set twice. Original value will be overwritten. Are you sure this is what you wanted?");
			}

			onClickCreateProject = func_;
		}

	private:
		std::string projectName;
		std::string projectPath;

		Callback onExit;
		Callback onClickCreateProject;
	};
}

#endif //!GADGET_WORKBENCH_WINDOWS_CREATE_PROJECT_WINDOW_H