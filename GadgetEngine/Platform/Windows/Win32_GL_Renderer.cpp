#include "Win32_GL_Renderer.h"

#include <glad/glad.h>

#include "App.h"
#include "Config.h"
#include "Debug.h"
#include "Game/Scene.h"
#include "Graphics/Components/CameraComponent.h"
#include "Graphics/Components/LightComponent.h"
#include "Graphics/Components/RenderComponent.h"
#include "Graphics/Components/SkyboxComponent.h"
#include "Graphics/OpenGL/GL_DynamicMeshInfo.h"
#include "Graphics/OpenGL/GL_MeshInfo.h"
#include "Graphics/OpenGL/GL_TextureInfo.h"
#include "Graphics/OpenGL/GL_FontInfo.h"
#include "Graphics/GUI/CanvasSceneComponent.h"
#include "Graphics/GUI/GuiTextElement.h"
#include "Graphics/GUI/GuiTextureElement.h"
#include "Graphics/Text/TextMesh.h"
#include "Platform/Windows/Win32_Window.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

Win32_GL_Renderer::Win32_GL_Renderer(int w_, int h_, int x_, int y_) : Renderer(API::OpenGL), glContext(nullptr), mainFBO(nullptr), screenShader(nullptr), screenQuad(nullptr){
	GADGET_BASIC_ASSERT(w_ > 0);
	GADGET_BASIC_ASSERT(h_ > 0);

	window = std::make_unique<Win32_Window>(w_, h_, x_, y_, renderAPI);

	GADGET_ASSERT(dynamic_cast<Win32_Window*>(window.get()) != nullptr, "Win32 Renderer requires a Win32 window!");
	glContext = SDL_GL_CreateContext(dynamic_cast<Win32_Window*>(window.get())->GetSDLWindow());
	if(glContext == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "OpenGL context could not be created! SDL Error: " + std::string(SDL_GetError()), ErrorCode::OpenGL_Error, __FILE__, __LINE__);
	}

	int swapInterval = App::GetConfig().GetOptionBool(EngineVars::Display::vsyncKey) ? -1 : 0;
	int status = SDL_GL_SetSwapInterval(swapInterval);
	if(status != 0 && swapInterval < 0){
		//Adaptive sync isn't supported, try again with regular vsync
		status = SDL_GL_SetSwapInterval(1);
	}

	if(status != 0){
		Debug::Log("Swap interval could not be set! SDL Error: " + std::string(SDL_GetError()), Debug::Error, __FILE__, __LINE__);
	}

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to initialize Glad! SDL Error: " + std::string(SDL_GetError()), ErrorCode::OpenGL_Error,  __FILE__, __LINE__);
	}

	SetWindingOrder(currentWindingOrder);
	SetCullFace(currentCullFace);

	SetViewportRect(ViewportRect::Fullscreen);
	SetClearColor(Color::DarkGray());

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ClearScreen();
	window->SwapBuffers();

	#ifdef GADGET_DEBUG
	if(glDebugMessageCallback){
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC)Win32_GL_Renderer::GLDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
	}else{
		Debug::Log(SID("RENDER"), "Could not set up OpenGL debug callback, OpenGL issues will not be logged!", Debug::Warning, __FILE__, __LINE__);
	}
	#endif // GADGET_DEBUG
}

Win32_GL_Renderer::~Win32_GL_Renderer(){
	delete screenQuad;
	App::GetResourceManager().UnloadResource(SID("ScreenShader"));
	delete mainFBO;

	SDL_GL_DeleteContext(glContext);

	window.reset();
}

void Win32_GL_Renderer::PostInit(){
	screenShader = App::GetResourceManager().LoadResource<GL_Shader>(SID("ScreenShader"));
	screenQuad = new GL_ScreenQuad();
	mainFBO = new GL_DefaultFrameBuffer(window->GetWidth(), window->GetHeight());

	Renderer::PostInit();
}

void Win32_GL_Renderer::Render(const Scene* scene_){
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
	auto skybox = scene_->GetSceneComponent<SkyboxComponent>();
	auto canvas = scene_->GetSceneComponent<CanvasSceneComponent>();

	std::vector<GuiTextElement*> guiTexts;
	std::vector<GuiTextureElement*> guiTextures;
	if(canvas != nullptr){
		guiTexts = canvas->GetCanvas().GetElements<GuiTextElement>();
		guiTextures = canvas->GetCanvas().GetElements<GuiTextureElement>();
	}

	GADGET_ASSERT(lights.size() < GL_MAX_UNIFORM_LOCATIONS, "Too many light sources in this scene! Max allowed is " + std::to_string(GL_MAX_UNIFORM_LOCATIONS) + ", this scene has " + std::to_string(lights.size()) + "!");

	for(const auto& cam : cams){
		SetViewportRect(cam->GetCamera().GetViewportRect());
		Matrix4 view = cam->GetUpdatedViewMatrix();
		Matrix4 proj = cam->GetUpdatedProjectionMatrix();

		//------------------------------------------------------------------------------------------------------------------------
		//Render all meshes in the scene
		for(const auto& mesh : meshes){
			mesh->Bind();
			mesh->GetShader()->BindMatrix4(SID("projectionMatrix"), proj);
			mesh->GetShader()->BindMatrix4(SID("viewMatrix"), view);

			Matrix4 modelMatrix = mesh->GetParent()->GetTransformMatrix();
			mesh->GetShader()->BindMatrix4(SID("modelMatrix"), modelMatrix);

			if(mesh->GetMaterial()->HasLighting()){
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
			}

			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetMeshNumIndices()), GL_UNSIGNED_INT, nullptr);

			mesh->Unbind();
		}

		//------------------------------------------------------------------------------------------------------------------------
		//Render the skybox
		if(skybox != nullptr){
			glDepthFunc(GL_LEQUAL);
			skybox->Bind(proj, view.ToMatrix3().ToMatrix4());
		
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

			skybox->Unbind();
			glDepthFunc(GL_LESS);
		}

		//------------------------------------------------------------------------------------------------------------------------
		//Render UI elements

		Matrix4 uiProjection = Matrix4::Orthographic(-1.0f, 1.0f, -1.0f, 1.0f);
		uiProjection[0] = 1.0f / GetAspectRatio();

		//TEXT
		for(const auto& text : guiTexts){
			text->GetTextMesh().GetShader()->Bind();
			text->GetTextMesh().GetShader()->BindMatrix4(SID("projectionMatrix"), uiProjection);

			text->GetTextMesh().GetMeshInfo()->Bind();

			size_t totalWidthInPixels = text->GetTextMesh().GetTotalWidthInPixels();
			size_t totalHeightInPixels = text->GetTextMesh().GetTotalHeightInPixels();

			float screenWidthPerPixel = text->GetSize().x / totalWidthInPixels;
			float screenHeightPerPixel = screenWidthPerPixel;
			float heightToUse = screenWidthPerPixel * totalHeightInPixels;

			float x = text->GetPosition().x - (text->GetSize().x / 2.0f);
			float y = text->GetPosition().y - (heightToUse / 2.0f);

			GL_FontInfo* fontInfo = dynamic_cast<GL_FontInfo*>(text->GetTextMesh().GetFont()->GetFontInfo());
			GL_DynamicMeshInfo* meshInfo = dynamic_cast<GL_DynamicMeshInfo*>(text->GetTextMesh().GetMeshInfo());

			for(char c : text->GetText()){
				FreetypeFontCharacter ch = text->GetTextMesh().GetFont()->GetCharacters().at(c);

				GLfloat xpos = x + ch.left * screenWidthPerPixel;
				GLfloat ypos = y - (ch.rows - ch.top) * screenHeightPerPixel;

				GLfloat w = ch.width * screenWidthPerPixel;
				GLfloat h = ch.rows * screenHeightPerPixel;
				// Update VBO for each character
				GLfloat vertices[6][4] = {
					{ xpos,     ypos + h,   0.0, 0.0 },
					{ xpos,     ypos,		0.0, 1.0 },
					{ xpos + w, ypos,       1.0, 1.0 },

					{ xpos,     ypos + h,   0.0, 0.0 },
					{ xpos + w, ypos,       1.0, 1.0 },
					{ xpos + w, ypos + h,   1.0, 0.0 }
				};

				//Don't bind the texture if it's a space, for some reason doing so causes OpenGL warnings
				if(c != 32){
					//Render glyph texture over quad
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, fontInfo->GetTextureForChar(c));
				}

				//Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, meshInfo->GetVBO());
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				//Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				//Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += (ch.advanceX >> 6) * screenWidthPerPixel; //Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels)) 
			}

			text->GetTextMesh().GetMeshInfo()->Unbind();
			text->GetTextMesh().GetShader()->Unbind();
		}

		//TEXTURES
		for(const auto& texture : guiTextures){
			Shader* shader = texture->GetShader();
			shader->Bind();
			shader->BindMatrix4(SID("projectionMatrix"), uiProjection);

			GL_DynamicMeshInfo* meshInfo = dynamic_cast<GL_DynamicMeshInfo*>(texture->GetMeshInfo());
			meshInfo->Bind();

			float x = texture->GetPosition().x;
			float y = texture->GetPosition().y;
			float w = texture->GetSize().x;
			float h = texture->GetSize().y;

			GLfloat vertices[6][4] = {
				{ x - w, y + h, 0.0f, 0.0f },
				{ x - w, y - h, 0.0f, 1.0f },
				{ x + w, y - h, 1.0f, 1.0f },

				{ x - w, y + h, 0.0f, 0.0f },
				{ x + w, y - h, 1.0f, 1.0f },
				{ x + w, y + h, 1.0f, 0.0f }
			};

			GL_TextureInfo* textureInfo = dynamic_cast<GL_TextureInfo*>(texture->GetTextureInfo());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureInfo->GetTexture());

			//Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, meshInfo->GetVBO());
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	//------------------------------------------------------------------------------------------------------------------------
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

void Win32_GL_Renderer::ClearScreen(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Win32_GL_Renderer::SetClearColor(const Color& color_){
	GADGET_BASIC_ASSERT(color_.IsValid());
	glClearColor(color_.r, color_.g, color_.b, color_.a);
}

void Win32_GL_Renderer::SetViewportRect(const Rect& rect_){
	GADGET_BASIC_ASSERT(rect_.IsValid());
	GADGET_ASSERT(rect_.x >= 0.0f && rect_.x <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.y >= 0.0f && rect_.y <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.w >= 0.0f && rect_.w <= 1.0f, "Tried to set invalid viewport rect!");
	GADGET_ASSERT(rect_.h >= 0.0f && rect_.h <= 1.0f, "Tried to set invalid viewport rect!");

	glViewport(static_cast<GLint>(window->GetWidth() * rect_.x),
				static_cast<GLint>(window->GetHeight() * rect_.y),
				static_cast<GLsizei>(window->GetWidth() * rect_.w),
				static_cast<GLsizei>(window->GetHeight() * rect_.h));
}

void Win32_GL_Renderer::OnResize(int width_, int height_){
	GADGET_BASIC_ASSERT(width_ > 0);
	GADGET_BASIC_ASSERT(height_ > 0);

	if(!postInitComplete){
		return; //New size will be handled correctly when we finish initializing
	}

	if(mainFBO != nullptr){
		delete mainFBO;
		mainFBO = nullptr;
	}
	mainFBO = new GL_DefaultFrameBuffer(width_, height_);
}

void Win32_GL_Renderer::SetWindingOrder(WindingOrder order_){
	GADGET_BASIC_ASSERT(order_ < WindingOrder::WindingOrder_MAX);

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

void Win32_GL_Renderer::SetCullFace(CullFace cullFace_){
	GADGET_BASIC_ASSERT(cullFace_ < CullFace::CullFace_MAX);

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

Shader* Win32_GL_Renderer::GenerateAPIShader(StringID shaderResource_){
	GADGET_BASIC_ASSERT(shaderResource_ != StringID::None);
	return App::GetResourceManager().LoadResource<GL_Shader>(shaderResource_); //TODO - This feels bad...
}

MaterialInfo* Win32_GL_Renderer::GenerateAPIMaterialInfo([[maybe_unused]] const std::vector<Color>& colors_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

MeshInfo* Win32_GL_Renderer::GenerateAPIMeshInfo(const Mesh& mesh_){
	return new GL_MeshInfo(mesh_);
}

MeshInfo* Win32_GL_Renderer::GenerateAPIDynamicMeshInfo(size_t numVertices_, size_t numIndices_){
	return new GL_DynamicMeshInfo(numVertices_, numIndices_);
}

TextureInfo* Win32_GL_Renderer::GenerateAPITextureInfo(const Texture& texture_){
	return new GL_TextureInfo(texture_);
}

FontInfo* Win32_GL_Renderer::GenerateAPIFontInfo(const FreetypeFont& font_){
	return new GL_FontInfo(font_);
}

void __stdcall Win32_GL_Renderer::GLDebugCallback(GLenum source_, GLenum type_, GLuint id_, GLenum severity_, GLsizei, const GLchar* message_, const void*){
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