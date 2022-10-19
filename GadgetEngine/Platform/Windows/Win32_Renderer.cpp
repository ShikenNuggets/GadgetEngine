#include "Win32_Renderer.h"

#include <glad/glad.h>

#include "Win32_Window.h"
#include "Debug.h"
#include "Graphics/Components/CameraComponent.h"
#include "Graphics/Components/LightComponent.h"
#include "Graphics/Components/RenderComponent.h"
#include "Graphics/Loaders/BmpLoader.h"
#include "Graphics/Loaders/ObjLoader.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Resource/ResourceManager.h"

//Just for testing things - TODO, remove these later
#include "Core/Time.h"
#include "Input/Input.h"

using namespace Gadget;

Win32_Renderer::Win32_Renderer(int w_, int h_) : Renderer(API::OpenGL), glContext(nullptr), mainFBO(nullptr), screenShader(nullptr), screenQuad(nullptr), cubemap(nullptr), cubemapInfo(nullptr), skyboxShader(nullptr){
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
	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	ResourceManager::GetInstance()->UnloadResource(SID("SkyboxShader"));
	delete cubemapInfo;
	delete cubemap;

	delete screenQuad;
	ResourceManager::GetInstance()->UnloadResource(SID("ScreenShader"));
	delete mainFBO;

	SDL_GL_DeleteContext(glContext);

	window.reset();
}

void Win32_Renderer::PostInit(){
	screenShader = ResourceManager::GetInstance()->LoadResource<GL_Shader>(SID("ScreenShader"));
	screenQuad = new GL_ScreenQuad();
	mainFBO = new GL_DefaultFrameBuffer(window->GetWidth(), window->GetHeight());

	//SKYBOX RENDERING (TODO - Move this to a more permanent location)
	if(cubemap == nullptr){
		skyboxShader = GenerateAPIShader(SID("SkyboxShader"));
		cubemap = new Cubemap(SID("SkyboxRightTexture"), SID("SkyboxLeftTexture"), SID("SkyboxTopTexture"), SID("SkyboxBottomTexture"), SID("SkyboxFrontTexture"), SID("SkyboxBackTexture"));
		cubemapInfo = new GL_CubemapInfo(*cubemap);
	}

	Renderer::PostInit();
}

void Win32_Renderer::Render(const Scene* scene_){
	GADGET_BASIC_ASSERT(scene_ != nullptr);
	GADGET_ASSERT(postInitComplete, "PostInit has not been called!");

	mainFBO->Bind();
	glEnable(GL_DEPTH_TEST);

	SetViewportRect(ViewportRect::Fullscreen);
	ClearScreen();

	//TODO - This is very inefficient, find a better way to do this
	//TODO - However we actually decide to do this it should be part of the generic renderer instead of being here
	auto cams = scene_->GetAllComponentsInScene<CameraComponent>();
	auto meshes = scene_->GetAllComponentsInScene<RenderComponent>();
	auto lights = scene_->GetAllComponentsInScene<PointLightComponent>();

	GADGET_ASSERT(lights.size() < GL_MAX_UNIFORM_LOCATIONS, "Too many light sources in this scene! Max allowed is " + std::to_string(GL_MAX_UNIFORM_LOCATIONS) + ", this scene has " + std::to_string(lights.size()) + "!");

	for(const auto& cam : cams){
		SetViewportRect(cam->GetCamera().GetViewportRect());
		Matrix4 view = cam->GetCamera().GetViewMatrix();
		Matrix4 proj = cam->GetCamera().GetProjectionMatrix();

		for(const auto& mesh : meshes){
			mesh->Bind();
			mesh->GetShader()->BindMatrix4(SID("projectionMatrix"), proj);
			mesh->GetShader()->BindMatrix4(SID("viewMatrix"), view);

			Matrix4 modelMatrix = mesh->GetParent()->GetTransform();
			mesh->GetShader()->BindMatrix4(SID("modelMatrix"), modelMatrix);
			mesh->GetShader()->BindMatrix3(SID("normalMatrix"), (modelMatrix.Inverse()).Transpose().ToMatrix3());

			mesh->GetShader()->BindVector3(SID("viewPos"), cam->GetParent()->GetPosition());

			mesh->GetShader()->BindInt(SID("numPointLights"), static_cast<int>(lights.size()));
			mesh->GetShader()->BindInt(SID("numSpotLights"), 0);
			mesh->GetShader()->BindInt(SID("numDirLights"), 0);

			for(const auto& light : lights){
				mesh->GetShader()->BindVector3(SID("pointLights[0].position"), light->GetParent()->GetPosition());
				mesh->GetShader()->BindColor(SID("pointLights[0].lightColor"), light->GetLightSource().GetColor());
				mesh->GetShader()->BindFloat(SID("pointLights[0].constant"), light->GetLightSource().GetConstant());
				mesh->GetShader()->BindFloat(SID("pointLights[0].linear"), light->GetLightSource().GetLinear());
				mesh->GetShader()->BindFloat(SID("pointLights[0].quadratic"), light->GetLightSource().GetQuadratic());
			}

			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetMeshNumIndices()), GL_UNSIGNED_INT, nullptr);

			mesh->Unbind();
		}

		//SKYBOX RENDERING (TODO - Reorganize this so the scene controls the Skybox, or, something. Anything but this)
		glDepthFunc(GL_LEQUAL);
		skyboxShader->Bind();
		skyboxShader->BindMatrix4(SID("projectionMatrix"), cam->GetCamera().GetProjectionMatrix());
		Matrix4 skyView = cam->GetCamera().GetViewMatrix().ToMatrix3().ToMatrix4();
		skyboxShader->BindMatrix4(SID("viewMatrix"), skyView);
		cubemapInfo->Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
		cubemapInfo->Unbind();
		skyboxShader->Unbind();
		glDepthFunc(GL_LESS);
		//END SKYBOX RENDERING
	}

	//Second Render Pass
	mainFBO->Unbind();
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	SetViewportRect(ViewportRect::Fullscreen);

	screenShader->Bind();
	screenQuad->Bind();
	glBindTexture(GL_TEXTURE_2D, mainFBO->GetColorTexture());
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	screenQuad->Unbind();
	screenShader->Unbind();

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

void Win32_Renderer::OnResize(int width_, int height_){
	if(!postInitComplete){
		return; //New size will be handled correctly when we finish initializing
	}

	if(mainFBO != nullptr){
		delete mainFBO;
		mainFBO = nullptr;
	}
	mainFBO = new GL_DefaultFrameBuffer(width_, height_);

	//TODO - Update camera aspect ratios when this happens
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
	return ResourceManager::GetInstance()->LoadResource<GL_Shader>(shaderResource_); //TODO - This feels bad...
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
		case 131169: //The driver allocated storage for renderbuffer - This should not be considered a warning
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