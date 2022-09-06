#include "Win32_Renderer.h"

#include <glad/glad.h>

#include "Win32_Window.h"
#include "Debug.h"
#include "Graphics/Loaders/ObjLoader.h"
#include "Resource/ResourceManager.h"

#include "Core/Time.h"

using namespace Gadget;

Win32_Renderer::Win32_Renderer(int w_, int h_) : Renderer(), mesh(nullptr), meshInfo(nullptr), shader(nullptr){
	window = std::make_unique<Win32_Window>(w_, h_);

	_ASSERT(dynamic_cast<Win32_Window*>(window.get()) != nullptr);
	glContext = SDL_GL_CreateContext(dynamic_cast<Win32_Window*>(window.get())->GetSDLWindow());
	if(glContext == nullptr){
		Debug::Log("OpenGL context could not be created! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		Debug::Log("Failed to initialize Glad! SDL Error: " + std::string(SDL_GetError()), Debug::LogType::FatalError, __FILE__, __LINE__);
		//TODO - Handle Fatal Error
	}

	SetWindingOrder(currentWindingOrder);
	SetCullFace(currentCullFace);

	SetViewportRect(ViewportRect::Fullscreen);
	SetClearColor(Color::DarkGray());

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	ClearScreen();
	window->SwapBuffers();

	#ifdef GADGET_DEBUG
	if(glDebugMessageCallback){
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC)Win32_Renderer::GLDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
	}else{
		Debug::Log(SID("RENDER"), "Could not set up OpenGL debug callback, OpenGL issues will not be logged!", Debug::Warning, __FILE__, __LINE__);
	}
	#endif // GADGET_DEBUG

	//MODEL RENDERING
	//TODO - Obviously get rid of this code Soon(TM)
	mesh = ObjLoader::LoadMesh("Resources/cube.obj");
	meshInfo = new GL_MeshInfo(*mesh);
	shader = ResourceManager::GetInstance()->LoadResource<GL_Shader>(SID("DefaultShader"));
}

Win32_Renderer::~Win32_Renderer(){
	delete meshInfo;
	meshInfo = nullptr;

	SDL_GL_DeleteContext(glContext);

	window.reset();
}

void Win32_Renderer::Render(){
	ClearScreen();

	//MODEL RENDERING
	//TODO - Obviously get rid of this code Soon(TM)
	meshInfo->Bind();
	shader->Bind();

	shader->BindMatrix4(SID("projectionMatrix"), Matrix4::Perspective(45.0f, GetAspectRatio(), 0.1f, 100.0f));
	shader->BindMatrix4(SID("viewMatrix"), Matrix4::LookAt(Vector3(0.0f, 0.0f, 4.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)));

	Matrix4 mm = Matrix4::Identity();
	mm *= Matrix4::Rotate(Time::GetInstance()->TimeSinceStartup() * 10.0f, Vector3(0.0f, 1.0f, 0.0f));
	mm *= Matrix4::Rotate(Time::GetInstance()->TimeSinceStartup() * 10.0f, Vector3(1.0f, 0.0f, 0.0f));

	shader->BindMatrix4(SID("modelMatrix"), mm);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->vertices.size()), GL_UNSIGNED_INT, nullptr);

	shader->Unbind();
	meshInfo->Unbind();

	//Do this only at the end
	window.get()->SwapBuffers();
}

void Win32_Renderer::ClearScreen(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Win32_Renderer::SetClearColor(const Color& color_){
	glClearColor(color_.r, color_.g, color_.b, color_.a);
}

void Win32_Renderer::SetViewportRect(const Rect& rect_){
	//Invalid window rect
	_ASSERT(rect_.x >= 0.0f && rect.x <= 1.0f);
	_ASSERT(rect_.y >= 0.0f && rect.y <= 1.0f);
	_ASSERT(rect_.w >= 0.0f && rect.w <= 1.0f);
	_ASSERT(rect_.h >= 0.0f && rect.h <= 1.0f);

	glViewport(static_cast<GLint>(window->GetWidth() * rect_.x),
				static_cast<GLint>(window->GetHeight() * rect_.y),
				static_cast<GLsizei>(window->GetWidth() * rect_.w),
				static_cast<GLsizei>(window->GetHeight() * rect_.h));
}

void Win32_Renderer::SetWindingOrder(WindingOrder order_){
	Renderer::SetWindingOrder(order_);

	switch(currentWindingOrder){
		case WindingOrder::Clockwise:
			glFrontFace(GL_CW);
			break;
		case WindingOrder::CounterClockwise:
			glFrontFace(GL_CCW);
			break;
		default:
			Debug::Log(SID("[RENDER]"), "Unsupported winding order [" + std::to_string((int)currentWindingOrder) + "]!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}

void Win32_Renderer::SetCullFace(CullFace cullFace_){
	Renderer::SetCullFace(cullFace_);

	switch(currentCullFace){
		case CullFace::None:
			glDisable(GL_CULL_FACE);
			break;
		case CullFace::Back:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case CullFace::Front:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		case CullFace::All:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
			break;
		default:
			Debug::Log(SID("[RENDER]"), "Unsupported cull face [" + std::to_string((int)currentCullFace) + "]!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}

void __stdcall Win32_Renderer::GLDebugCallback(GLenum source_, GLenum type_, GLuint id_, GLenum severity_, GLsizei, const GLchar* message_, const void*){
	//Suppress useless messages
	switch(id_){
		case 131185: //Buffer object will use VIDEO memory - Irrelevant
			return;
		default:
			break;
	}

	std::string finalMessage;

	switch(source_){
		case GL_DEBUG_SOURCE_API:
			finalMessage += ("API ");
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			finalMessage += ("Window System ");
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			finalMessage += ("Third Party ");
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			finalMessage += ("Application ");
			break;
		case GL_DEBUG_SOURCE_OTHER:
			break;
		default:
			break;
	}

	switch(type_){
		case GL_DEBUG_TYPE_ERROR:
			finalMessage += ("Error ");
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			finalMessage += ("Deprecated Behaviour ");
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			finalMessage += ("Undefined Behaviour ");
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			finalMessage += ("Portability ");
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			finalMessage += ("Performance ");
			break;
		case GL_DEBUG_TYPE_MARKER:
			finalMessage += ("Marker ");
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			finalMessage += ("Push Group ");
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			finalMessage += ("Pop Group ");
			break;
		case GL_DEBUG_TYPE_OTHER:
			//Intentional Fallthrough
		default:
			finalMessage += ("Other ");
			break;
	}

	finalMessage += std::to_string(id_) + ": " + message_;

	switch(severity_){
		case GL_DEBUG_SEVERITY_HIGH:
			Debug::Log(SID("OPENGL"), finalMessage, Debug::Error);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			Debug::Log(SID("OPENGL"), finalMessage, Debug::Warning);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			Debug::Log(SID("OPENGL"), finalMessage, Debug::Warning);
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			Debug::Log(SID("OPENGL"), finalMessage, Debug::Verbose);
			break;
	}
}