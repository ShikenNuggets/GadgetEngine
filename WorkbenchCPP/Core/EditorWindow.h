#ifndef WORKBENCH_CORE_EDITOR_WINDOW_H
#define WORKBENCH_CORE_EDITOR_WINDOW_H

#include <SDL.h>

namespace Workbench{
	class EditorWindow{
	public:
		EditorWindow(int width_, int height_);
		~EditorWindow();

		bool HandleEvents();
		void Render();

	private:
		SDL_Window* window;
		SDL_GLContext glContext;
	};
}

#endif //!WORKBENCH_CORE_EDITOR_WINDOW_H