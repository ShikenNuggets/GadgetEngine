#ifndef GADGET_WORKBENCH_CORE_EDITOR_WINDOW_H
#define GADGET_WORKBENCH_CORE_EDITOR_WINDOW_H

#include <SDL.h>

namespace Gadget::Workbench{
	class EditorWindow{
	public:
		EditorWindow(int width_, int height_);
		~EditorWindow();

		bool HandleEvents();
		void Render();

	private:
		SDL_Window* window;
		SDL_GLContext glContext;

		uint64_t GetWindowHandle() const;
	};
}

#endif //!GADGET_WORKBENCH_CORE_EDITOR_WINDOW_H