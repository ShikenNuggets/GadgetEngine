#include "Win32_Window.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "App.h"
#include "Debug.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Input/GamepadEvent.h"
#include "Platform/PlatformUtils.h"
#include "Platform/Windows/SDL2_Utils.h"

using namespace Gadget;

static constexpr int gGLMajorVersion = 4;
static constexpr int gGLMinorVersion = 6;

Win32_Window::Win32_Window(int w_, int h_, int x_, int y_, Renderer::API renderAPI_) : Window(w_, h_, x_, y_), sdlWindow(nullptr), joysticks(), refreshRate(0.0f){
	GADGET_BASIC_ASSERT(w_ > 0);
	GADGET_BASIC_ASSERT(h_ > 0);

	if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)){
		Debug::ThrowFatalError(SID("RENDER"), "SDL could not be initialized! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
	}

	Uint32 windowFlag = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
	if(renderAPI_ == Renderer::API::OpenGL){
		windowFlag |= SDL_WINDOW_OPENGL;

		if(!SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1)){
			Debug::ThrowFatalError(SID("RENDER"), "Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
		}

		if(!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gGLMajorVersion)){
			Debug::ThrowFatalError(SID("RENDER"), "Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
		}

		if(!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gGLMinorVersion)){
			Debug::ThrowFatalError(SID("RENDER"), "Issue with setting OpenGL attribute! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
		}
	}

	if(pos.x == 0 && pos.y == 0){
		pos.x = SDL_WINDOWPOS_CENTERED;
		pos.y = SDL_WINDOWPOS_CENTERED;
	}

	sdlWindow = SDL_CreateWindow(App::GetGameName().c_str(), /*pos.x, pos.y,*/ GetWidth(), GetHeight(), windowFlag);
	if(sdlWindow == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "Window could not be created! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
	}

	//This is enabled by default but it's good to be explicit
	SDL_SetJoystickEventsEnabled(true);
	if(!SDL_JoystickEventsEnabled()){
		Debug::ThrowFatalError(SID("INPUT"), "Joystick events could not be enabled! SDL Error: " + std::string(SDL_GetError()), ErrorCode::SDL_Error, __FILE__, __LINE__);
	}

	const SDL_DisplayID id = SDL_GetPrimaryDisplay();
	if (id == 0)
	{
		GADGET_LOG_WARNING(SID("RENDER"), "Could not get primary display ID! SDL Error: " + std::string(SDL_GetError()));
	}
	else
	{
		const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(id);
		if(mode == nullptr){
			Debug::Log("Could not get Display Mode! SDL Error: " + std::string(SDL_GetError()), Debug::Error, __FILE__, __LINE__);
		}else{
			refreshRate = mode->refresh_rate;
		}
	}

	PlatformUtils::TryApplyImmersiveDarkMode(GetWindowHandle());
}

Win32_Window::~Win32_Window(){
	if(sdlWindow != nullptr){
		SDL_DestroyWindow(sdlWindow);
		sdlWindow = nullptr;
	}

	//Close the SDL Subsystem
	SDL_Quit();
}

uint64_t Win32_Window::GetWindowHandle() const{
	return reinterpret_cast<uint64_t>(SDL_GetPointerProperty(SDL_GetWindowProperties(sdlWindow), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL));
}

SDL_Window* Win32_Window::GetSDLWindow() const{ return sdlWindow; }

void Win32_Window::HandleEvents(){
	//Variables that will be used in the switch
	SDL_Joystick* joystick = nullptr;
	AxisID joystickAxis = AxisID::None;
	float range = 0.0f;

	SDL_Event e;
	while(SDL_PollEvent(&e)){
		switch(e.type){
			case SDL_EVENT_QUIT:
				App::CloseGame();
				return;
			case SDL_EVENT_WINDOW_RESIZED:
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			case SDL_EVENT_WINDOW_MOVED:
				HandleWindowEvent(e);
				break;
			case SDL_EVENT_KEY_DOWN:
				if(e.key.repeat == 0){ //Ignore key repeats - TODO - maybe repeats should be a separate keyboard event?
					EventHandler::GetInstance()->HandleEvent(KeyPressedEvent(SDL2_Utils::ConvertSDLKeycodeToButtonID(e.key.key)));
				}
				break;
			case SDL_EVENT_KEY_UP:
				EventHandler::GetInstance()->HandleEvent(KeyReleasedEvent(SDL2_Utils::ConvertSDLKeycodeToButtonID(e.key.key)));
				break;
			case SDL_EVENT_MOUSE_MOTION:
				GADGET_BASIC_ASSERT(GetWidth() > 0);
				GADGET_BASIC_ASSERT(GetHeight() > 0);
				EventHandler::GetInstance()->HandleEvent(MouseMovedEvent(static_cast<float>(e.motion.xrel) / static_cast<float>(GetWidth()), static_cast<float>(e.motion.yrel) / static_cast<float>(GetHeight()), static_cast<float>(e.motion.x), static_cast<float>(e.motion.y)));
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				EventHandler::GetInstance()->HandleEvent(MouseButtonPressedEvent(SDL2_Utils::ConvertSDLMouseButtonToButtonID(e.button.button)));
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				EventHandler::GetInstance()->HandleEvent(MouseButtonReleasedEvent(SDL2_Utils::ConvertSDLMouseButtonToButtonID(e.button.button)));
				break;
			case SDL_EVENT_MOUSE_WHEEL:
				EventHandler::GetInstance()->HandleEvent(MouseScrollEvent(e.wheel.mouse_x, e.wheel.mouse_y));
				break;
			case SDL_EVENT_JOYSTICK_ADDED:
				joystick = SDL_OpenJoystick(e.jdevice.which);
				joysticks.push_back(joystick);
				Debug::Log("Gamepad " + std::to_string(e.jdevice.which) + " connected.");
				break;
			case SDL_EVENT_JOYSTICK_REMOVED:
				RemoveJoystick(e.jdevice.which);
				Debug::Log("Gamepad " + std::to_string(e.jdevice.which) + " disconnected.");
				break;
			//TODO - None of the following joystick events know or care WHICH joystick is being used
			//For a singleplayer game this doesn't really matter but this is a HUGE problem for splitscreen
			case SDL_EVENT_JOYSTICK_AXIS_MOTION:
				joystickAxis = SDL2_Utils::ConvertSDLJoystickAxisToAxisID(e.jaxis.axis);
				range = static_cast<float>(e.jaxis.value) / static_cast<float>(std::numeric_limits<int16_t>::max()); //Convert the quantized value we get from SDL to a float between -1 and 1
				
				if(joystickAxis == AxisID::Gamepad_LeftStick_Vertical || joystickAxis == AxisID::Gamepad_RightStick_Vertical){
					range *= -1.0f;
				}
				
				EventHandler::GetInstance()->HandleEvent(GamepadAxisEvent(e.jaxis.which, joystickAxis, range));
				//TODO - Trigger button events here as well
				break;
			case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
				EventHandler::GetInstance()->HandleEvent(GamepadButtonPressedEvent(e.jbutton.which, SDL2_Utils::ConvertSDLJoystickButtonToButtonID(e.jbutton.button)));
				break;
			case SDL_EVENT_JOYSTICK_BUTTON_UP:
				EventHandler::GetInstance()->HandleEvent(GamepadButtonReleasedEvent(e.jbutton.which, SDL2_Utils::ConvertSDLJoystickButtonToButtonID(e.jbutton.button)));
				break;
			case SDL_EVENT_JOYSTICK_HAT_MOTION:
				HandleHatMotionEvent(e);
				break;
			default:
				break;
		}
	}
}

void Win32_Window::SwapBuffers(){
	SDL_GL_SwapWindow(sdlWindow);
}

void Win32_Window::HandleWindowEvent(const SDL_Event& e_){
	int w = 0, h = 0;
	ErrorCode err = ErrorCode::OK;
	switch(e_.type){
		case SDL_EVENT_WINDOW_RESIZED:
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			SDL_GetWindowSize(sdlWindow, &w, &h);
			size.x = w;
			size.y = h;
			if(renderSurface != nullptr){
				err = renderSurface->SetSize(size);
			}
			
			if(err != ErrorCode::OK){
				GADGET_LOG_ERROR(SID("CORE"), "An error occurred on resizing the render surface!");
			}

			EventHandler::GetInstance()->HandleEvent(WindowResizedEvent(w, h));
			break;
		case SDL_EVENT_WINDOW_MOVED:
			SDL_GetWindowPosition(sdlWindow, &w, &h);
			EventHandler::GetInstance()->HandleEvent(WindowMovedEvent(w, h));
			break;
		default:
			break;
	}
}

void Win32_Window::HandleHatMotionEvent(const SDL_Event& e_){
	if(e_.jhat.hat != 0){
		return; //TODO - Handling for multiple hats, for now we'll just ignore them. Most gamepads only have one
	}

	float xAxis = 0.0f;
	float yAxis = 0.0f;
	ButtonID button = ButtonID::None;

	if(e_.jhat.value & SDL_HAT_UP){
		yAxis = 1.0f;
		button = ButtonID::Gamepad_DPad_Up;
	}else if(e_.jhat.value & SDL_HAT_DOWN){
		yAxis = -1.0f;
		button = ButtonID::Gamepad_DPad_Down;
	}
	
	if(e_.jhat.value & SDL_HAT_RIGHT){
		xAxis = 1.0f;
		button = ButtonID::Gamepad_DPad_Right;
	}else if(e_.jhat.value & SDL_HAT_LEFT){
		xAxis = -1.0f;
		button = ButtonID::Gamepad_DPad_Left;
	}

	if(button != ButtonID::None){
		EventHandler::GetInstance()->HandleEvent(GamepadButtonPressedEvent(e_.jhat.which, button));
		EventHandler::GetInstance()->HandleEvent(GamepadButtonReleasedEvent(e_.jhat.which, button)); //TODO - This isn't quite right
	}
	
	EventHandler::GetInstance()->HandleEvent(GamepadAxisEvent(e_.jhat.which, AxisID::Gamepad_DPad_Horizontal, xAxis));
	EventHandler::GetInstance()->HandleEvent(GamepadAxisEvent(e_.jhat.which, AxisID::Gamepad_DPad_Vertical, yAxis));
}

//TODO - I'm honestly not really sure that this is correct, the documentation is a little vague on all this. Validate this
void Win32_Window::RemoveJoystick(SDL_JoystickID instanceID_){
	GADGET_ASSERT(joysticks.size() <= 256, "More than 256 joysticks are connected, something has likely gone wrong"); //Everything *should* work fine for up to int max, but I cannot imagine a scenario where we need that many

	int index = -1;
	for(int i = 0; i < joysticks.size(); i++){
		if(joysticks[i] == nullptr){
			continue;
		}

		auto result = SDL_GetJoystickID(joysticks[i]);
		if(result < 0){
			Debug::Log("Could not query Joystick Instance ID for joystick " + std::to_string(i) + "! SDL Error: " + std::string(SDL_GetError()), Debug::Error, __FILE__, __LINE__);
			continue;
		}

		if(result == instanceID_){
			index = i;
			break;
		}
	}

	SDL_CloseJoystick(joysticks[index]);
	joysticks[index] = nullptr;
}