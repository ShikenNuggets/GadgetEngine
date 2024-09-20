#include "EditorWindow.h"

#include <SDL_syswm.h>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>

#include <Debug.h>
#include <Math/Math.h>
#include <Platform/Windows/Win32_Utils.h>

using namespace Gadget::Workbench;

//These are somewhat arbitrary
static constexpr int gLowerSizeBound = 400;
static constexpr int gUpperSizeBound = 8000;

static constexpr int gGLMajorVersion = 4;
static constexpr int gGLMinorVersion = 6;

EditorWindow::EditorWindow(int width_, int height_) : window(nullptr), glContext(nullptr){
	GADGET_BASIC_ASSERT(width_ > 0);
	GADGET_BASIC_ASSERT(height_ > 0);

	int width = Math::Clamp(gLowerSizeBound, gUpperSizeBound, width_);
	int height = Math::Clamp(gLowerSizeBound, gUpperSizeBound, height_);

	if(SDL_Init(SDL_INIT_EVERYTHING) > 0){
		Debug::ThrowFatalError(SID("RENDER"), "SDL could not be initialized! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
	}

	Uint32 windowFlag = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
	if(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) != 0){
		Debug::ThrowFatalError(SID("RENDER"), "Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
	}

	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gGLMajorVersion) != 0){
		Debug::ThrowFatalError(SID("RENDER"), "Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
	}

	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gGLMinorVersion) != 0){
		Debug::ThrowFatalError(SID("RENDER"), "Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
	}

	window = SDL_CreateWindow("Workbench", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlag);
	if(window == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Window could not be created! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
	}

	glContext = SDL_GL_CreateContext(window);
	if(glContext == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "OpenGL context could not be created! SDL Error: " + std::string(SDL_GetError()), ErrorCode::OpenGL_Error, __FILE__, __LINE__);
	}

	if(!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress))){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to initialize Glad! SDL Error: " + std::string(SDL_GetError()), ErrorCode::OpenGL_Error, __FILE__, __LINE__);
	}

	Win32_Utils::TryApplyImmersiveDarkMode(GetWindowHandle());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

#ifdef GADGET_DEBUG
	io.ConfigDebugHighlightIdConflicts = true;
	io.ConfigDebugIniSettings = true;
#endif //GADGET_DEBUG

#ifdef GADGET_RELEASE
	io.ConfigDebugHighlightIdConflicts = false;
	io.ConfigDebugIniSettings = false;
#endif //GADGET_RELEASE

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init();
}

EditorWindow::~EditorWindow(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool EditorWindow::HandleEvents(){
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0){
		ImGui_ImplSDL2_ProcessEvent(&e);

		if(e.type == SDL_QUIT){
			return false;
		}
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	return true;
}

void EditorWindow::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(window);
}

uint64_t EditorWindow::GetWindowHandle() const{
	SDL_SysWMinfo wmInfo{};
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	return reinterpret_cast<uint64_t>(wmInfo.info.win.window);
}