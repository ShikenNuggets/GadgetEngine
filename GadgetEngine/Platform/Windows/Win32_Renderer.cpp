#include "Win32_Renderer.h"

#include <glad/glad.h>

#include "Win32_Window.h"
#include "Debug.h"
#include "Graphics/Loaders/BmpLoader.h"
#include "Graphics/Loaders/ObjLoader.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Resource/ResourceManager.h"

#include "Core/Time.h"

using namespace Gadget;

Win32_Renderer::Win32_Renderer(int w_, int h_) : Renderer(API::OpenGL), mesh(nullptr), meshInfo(nullptr), material(nullptr), camera(nullptr), light(nullptr){
	window = std::make_unique<Win32_Window>(w_, h_);

	GADGET_ASSERT(dynamic_cast<Win32_Window*>(window.get()) != nullptr, "Win32 Renderer requires a Win32 window!");
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

	glEnable(GL_TEXTURE_2D);
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
}

Win32_Renderer::~Win32_Renderer(){
	delete light;
	delete camera;
	delete meshInfo;
	delete mesh;

	SDL_GL_DeleteContext(glContext);

	window.reset();
}

void Win32_Renderer::Render(){
	ClearScreen();
	SetViewportRect(ViewportRect::Fullscreen);

	//MODEL RENDERING
	//TODO - Obviously get rid of this code Soon(TM)
	if(camera == nullptr){
		camera = new Camera();
		camera->CalculateViewMatrix(Vector3(0.0f, 0.0f, 4.0f), Quaternion::Identity());
	}

	if(light == nullptr){
		light = new PointLight(Vector3(2.0f, 1.0f, 1.0f));
	}

	if(meshInfo == nullptr){
		mesh = ObjLoader::LoadMesh("Resources/cube.obj");
		meshInfo = new GL_MeshInfo(*mesh);
	}

	if(material == nullptr){
		material = new DiffuseTextureMaterial(SID("CubeTexture"), SID("DefaultShader"));
	}

	SetViewportRect(camera->GetViewportRect());
	Matrix4 view = camera->GetViewMatrix();
	Matrix4 proj = camera->GetProjectionMatrix();

	meshInfo->Bind();
	material->Bind();

	material->GetShader()->BindMatrix4(SID("projectionMatrix"), proj);
	material->GetShader()->BindMatrix4(SID("viewMatrix"), view);

	Matrix4 mm = Matrix4::Identity();
	mm *= Matrix4::Rotate(Time::GetInstance()->TimeSinceStartup() * 10.0f, Vector3(0.0f, 1.0f, 0.0f));
	mm *= Matrix4::Rotate(Time::GetInstance()->TimeSinceStartup() * 10.0f, Vector3(1.0f, 0.0f, 0.0f));

	material->GetShader()->BindMatrix4(SID("modelMatrix"), mm);
	material->GetShader()->BindMatrix3(SID("normalMatrix"), (mm.Inverse()).Transpose().ToMatrix3());

	material->GetShader()->BindVector3(SID("viewPos"), Vector3(0.0f, 0.0f, 4.0f));

	material->GetShader()->BindInt(SID("numPointLights"), 1);
	material->GetShader()->BindInt(SID("numSpotLights"), 0);
	material->GetShader()->BindInt(SID("numDirLights"), 0);

	material->GetShader()->BindVector3(SID("pointLights[0].position"), light->GetPosition());
	material->GetShader()->BindColor(SID("pointLights[0].lightColor"), light->GetColor());
	material->GetShader()->BindFloat(SID("pointLights[0].constant"), light->GetConstant());
	material->GetShader()->BindFloat(SID("pointLights[0].linear"), light->GetLinear());
	material->GetShader()->BindFloat(SID("pointLights[0].quadratic"), light->GetQuadratic());

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indices.size()), GL_UNSIGNED_INT, nullptr);

	material->Unbind();
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
	GADGET_ASSERT(rect_.x >= 0.0f && rect_.x <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.y >= 0.0f && rect_.y <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.w >= 0.0f && rect_.w <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.h >= 0.0f && rect_.h <= 1.0f, "Tried to set invalid viewport rect!");

	glViewport(static_cast<GLint>(window->GetWidth() * rect_.x),
				static_cast<GLint>(window->GetHeight() * rect_.y),
				static_cast<GLsizei>(window->GetWidth() * rect_.w),
				static_cast<GLsizei>(window->GetHeight() * rect_.h));
}

void Win32_Renderer::OnResize(int, int){
	//TODO - This is tied to the hardcoded model rendering code. Obviously we need to handle this differently later
	if(camera != nullptr){
		camera->SetAspect(GetAspectRatio());
	}
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

Shader* Win32_Renderer::GenerateAPIShader(StringID shaderResource_){
	return ResourceManager::GetInstance()->LoadResource<GL_Shader>(shaderResource_); //This feels not right...
}

MeshInfo* Win32_Renderer::GenerateAPIMeshInfo(const Mesh& mesh_){
	return new GL_MeshInfo(mesh_);
}

TextureInfo* Win32_Renderer::GenerateAPITextureInfo(const Texture& texture_){
	return new GL_TextureInfo(texture_);
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