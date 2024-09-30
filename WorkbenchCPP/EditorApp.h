#ifndef GADGET_WORKBENCH_EDITOR_APP_H
#define GADGET_WORKBENCH_EDITOR_APP_H

#include "Core/EditorWindow.h"
#include "Core/FontManager.h"
#include "Core/WindowManager.h"
#include "Flows/Flow.h"
#include "Projects/ProjectManager.h"

namespace Gadget::Workbench{
	class EditorApp{
	public:
		EditorApp();
		~EditorApp() = default;

		explicit EditorApp(const EditorApp&) = delete;
		explicit EditorApp(EditorApp&&) = delete;
		EditorApp& operator=(const EditorApp&) = delete;
		EditorApp& operator=(EditorApp&&) = delete;

		static EditorApp& GetInstance();
		static void DeleteInstance();

		void Run();
		static void Close();

		static uint64_t GetMainWindowHandle();
		static ImFont* GetFont(StringID name_){ return GetInstance().fontManager.Get(name_); }

		static EditorWindow& GetEditorWindow(){ return GetInstance().window; }
		static ProjectManager& GetProjectManager(){ return GetInstance().projManager; }
		static WindowManager& GetWndManager(){ return GetInstance().wndManager; }

	private:
		static std::unique_ptr<EditorApp> instance;

		bool isRunning;
		EditorWindow window;
		ProjectManager projManager;
		WindowManager wndManager;
		FontManager fontManager;
		Flow* currentFlow;
	};
}

#endif //!GADGET_WORKBENCH_EDITOR_APP_H