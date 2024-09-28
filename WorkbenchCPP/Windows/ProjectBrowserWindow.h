#ifndef GADGET_WORKBENCH_PROJECT_BROWSER_WINDOW_H
#define GADGET_WORKBENCH_PROJECT_BROWSER_WINDOW_H

#include "Core/Callback.h"
#include "Projects/ProjectManager.h"
#include "Windows/SubWindow.h"

namespace Gadget::Workbench{
	class ProjectBrowserWindow : public SubWindow{
	public:
		ProjectBrowserWindow(const ProjectManager& projectManager_);

		void EnableInput(){ allowInput = true; }
		void DisableInput(){ allowInput = false; }

		virtual void Draw() override;

		void OnClickCreateProject(Callback func_){
			GADGET_BASIC_ASSERT(func_.IsValid());
			GADGET_BASIC_ASSERT(!onClickCreateProject.IsValid());
			if(onClickCreateProject.IsValid()){
				GADGET_LOG_WARNING(SID("PROJECT_BROWSER"), "OnClickCreateProject handler was set twice. Original value will be overwritten. Are you sure this is what you wanted?");
			}

			onClickCreateProject = func_;
		}

		void OnClickOpenProject(Callback func_){
			GADGET_BASIC_ASSERT(func_.IsValid());
			GADGET_BASIC_ASSERT(!onClickOpenProject.IsValid());
			if(onClickOpenProject.IsValid()){
				GADGET_LOG_WARNING(SID("PROJECT_BROWSER"), "OnClickOpenProject handler was set twice. Original value will be overwritten. Are you sure this is what you wanted?");
			}

			onClickOpenProject = func_;
		}

	private:
		bool allowInput;
		const ProjectManager& projectManager;
		Callback onClickCreateProject;
		Callback onClickOpenProject;
	};
}

#endif //!GADGET_WORKBENCH_PROJECT_BROWSER_WINDOW_H