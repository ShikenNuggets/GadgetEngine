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
#include "Utils/SIDArrayCache.h"

using namespace Gadget;

static inline SIDArrayCache gBoneIDs = SIDArrayCache("bones[", "]");

static inline SIDArrayCache gPointPositions		= SIDArrayCache("pointLights[", "].position");
static inline SIDArrayCache gPointLightColors	= SIDArrayCache("pointLights[", "].lightColor");
static inline SIDArrayCache gPointConstants		= SIDArrayCache("pointLights[", "].constant");
static inline SIDArrayCache gPointLinears		= SIDArrayCache("pointLights[", "].linear");
static inline SIDArrayCache gPointQuadratics	= SIDArrayCache("pointLights[", "].quadratic");

static inline SIDArrayCache gSpotPositions		= SIDArrayCache("spotLights[", "].position");
static inline SIDArrayCache gSpotDirections		= SIDArrayCache("spotLights[", "].direction");
static inline SIDArrayCache gSpotCutOffs		= SIDArrayCache("spotLights[", "].cutOff");
static inline SIDArrayCache gSpotOuterCutOffs	= SIDArrayCache("spotLights[", "].outerCutOff");
static inline SIDArrayCache gSpotLightColors	= SIDArrayCache("spotLights[", "].lightColor");
static inline SIDArrayCache gSpotConstants		= SIDArrayCache("spotLights[", "].constant");
static inline SIDArrayCache gSpotLinears		= SIDArrayCache("spotLights[", "].linear");
static inline SIDArrayCache gSpotQuadratics		= SIDArrayCache("spotLights[", "].quadratic");

static inline SIDArrayCache gDirDirections	= SIDArrayCache("dirLights[", "].direction");
static inline SIDArrayCache gDirLightColors	= SIDArrayCache("dirLights[", "].lightColor");

Win32_GL_Renderer::Win32_GL_Renderer(int w_, int h_, int x_, int y_) : Renderer(API::OpenGL), glContext(nullptr), mainFBO(nullptr), screenShader(nullptr), screenQuad(nullptr){
	GADGET_BASIC_ASSERT(w_ > 0);
	GADGET_BASIC_ASSERT(h_ > 0);

	window = std::make_unique<Win32_Window>(w_, h_, x_, y_, renderAPI);

	GADGET_ASSERT(dynamic_cast<Win32_Window*>(window.get()) != nullptr, "Win32 Renderer requires a Win32 window!");
	glContext = SDL_GL_CreateContext(dynamic_cast<Win32_Window*>(window.get())->GetSDLWindow());
	if(glContext == nullptr){
		Debug::ThrowFatalError(SID("RENDER"), "OpenGL context could not be created! SDL Error: " + std::string(SDL_GetError()), ErrorCode::OpenGL_Error, __FILE__, __LINE__);
	}

	const int swapInterval = App::GetConfig().GetOptionBool(EngineVars::Display::vsyncKey) ? -1 : 0;
	bool status = SDL_GL_SetSwapInterval(swapInterval);
	if(status && swapInterval < 0){
		//Adaptive sync isn't supported, try again with regular vsync
		status = SDL_GL_SetSwapInterval(1);
	}else if(!status){
		Debug::Log("Swap interval could not be set! SDL Error: " + std::string(SDL_GetError()), Debug::Error, __FILE__, __LINE__);
	}

	if(gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == GL_FALSE){
		Debug::ThrowFatalError(SID("RENDER"), "Failed to initialize Glad! SDL Error: " + std::string(SDL_GetError()), ErrorCode::OpenGL_Error,  __FILE__, __LINE__);
	}

	Win32_GL_Renderer::SetWindingOrder(currentWindingOrder);
	Win32_GL_Renderer::SetCullFace(currentCullFace);

	Win32_GL_Renderer::SetViewportRect(ViewportRect::Fullscreen);
	Win32_GL_Renderer::SetClearColor(Color::DarkGray());

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Win32_GL_Renderer::ClearScreen();
	window->SwapBuffers();

	#ifdef GADGET_DEBUG
	if(glDebugMessageCallback){
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(static_cast<GLDEBUGPROC>(Win32_GL_Renderer::GLDebugCallback), nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}else{
		Debug::Log(SID("RENDER"), "Could not set up OpenGL debug callback, OpenGL issues will not be logged!", Debug::Warning, __FILE__, __LINE__);
	}
	#endif // GADGET_DEBUG
}

Win32_GL_Renderer::~Win32_GL_Renderer(){
	RenderComponent::GetCollection().GetAllComponents(rendersBuffer);
	for(auto& render : rendersBuffer){
		render->InvalidateMeshInfo();
	}

	AnimRenderComponent::GetCollection().GetAllComponents(animRendersBuffer);
	for(auto& animRender : animRendersBuffer){
		animRender->InvalidateMeshInfo();
	}

	for(auto& pair : App::GetMaterialCache().GetMaterials()){
		pair.value->InvalidateAllAPIInfos();
	}

	delete screenQuad;
	App::GetResourceManager().UnloadResource(SID("ScreenShader"));
	delete mainFBO;

	App::GetResourceManager().ForceUnloadAssetsOfType<GL_Shader>();

	SDL_GL_DestroyContext(glContext); // TODO - Check return value

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
	scene_->GetAllComponentsInScene<CameraComponent>(camerasBuffer);
	scene_->GetAllComponentsInScene<RenderComponent>(rendersBuffer);
	scene_->GetAllComponentsInScene<AnimRenderComponent>(animRendersBuffer);
	scene_->GetAllComponentsInScene<PointLightComponent>(pointLightsBuffer);
	scene_->GetAllComponentsInScene<DirectionalLightComponent>(dirLightsBuffer);
	scene_->GetAllComponentsInScene<SpotLightComponent>(spotLightsBuffer);
	auto* skybox = scene_->GetSceneComponent<SkyboxComponent>();
	auto* canvas = scene_->GetSceneComponent<CanvasSceneComponent>();

	if(canvas != nullptr){
		canvas->GetCanvas().GetElements<GuiTextElement>(guiTextsBuffer);
		canvas->GetCanvas().GetElements<GuiTextureElement>(guiTexturesBuffer);
	}

	const size_t numLights = pointLightsBuffer.Size() + dirLightsBuffer.Size() + spotLightsBuffer.Size();
	GADGET_ASSERT(numLights < GL_MAX_UNIFORM_LOCATIONS, "Too many light sources in this scene! Max allowed is " + std::to_string(GL_MAX_UNIFORM_LOCATIONS) + ", this scene has " + std::to_string(numLights) + "!");

	for(const auto& cam : camerasBuffer){
		SetViewportRect(cam->GetCamera().GetViewportRect());
		const Matrix4 view = cam->GetUpdatedViewMatrix();
		const Matrix4 proj = cam->GetUpdatedProjectionMatrix();

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
		//Render all meshes in the scene
		static constexpr int maxLightSources = 8;

		for(const auto& mesh : rendersBuffer){
			for(size_t i = 0; i < mesh->GetNumSubmeshes(); i++){
				mesh->Bind(i);
				mesh->GetShader(i)->BindMatrix4(SID("projectionMatrix"), proj);
				mesh->GetShader(i)->BindMatrix4(SID("viewMatrix"), view);

				const Matrix4 modelMatrix = mesh->GetParent()->GetTransformMatrix();
				mesh->GetShader(i)->BindMatrix4(SID("modelMatrix"), modelMatrix);

				mesh->GetShader(i)->BindMatrix3(SID("normalMatrix"), (modelMatrix.Inverse()).Transpose().ToMatrix3());

				mesh->GetShader(i)->BindVector3(SID("viewPos"), cam->GetParent()->GetPosition());

				mesh->GetShader(i)->BindInt(SID("numPointLights"), static_cast<int>(pointLightsBuffer.Size()));
				mesh->GetShader(i)->BindInt(SID("numSpotLights"), static_cast<int>(spotLightsBuffer.Size()));
				mesh->GetShader(i)->BindInt(SID("numDirLights"), static_cast<int>(dirLightsBuffer.Size()));

				for(int64_t j = 0; j < pointLightsBuffer.Size() && j < maxLightSources; j++){
					const auto* light = pointLightsBuffer[j];
					GADGET_BASIC_ASSERT(light != nullptr && light->GetParent() != nullptr);
					mesh->GetShader(i)->BindVector3(gPointPositions.Get(j), light->GetParent()->GetPosition());
					mesh->GetShader(i)->BindColor(gPointLightColors.Get(j), light->GetLightSource().GetColor());
					mesh->GetShader(i)->BindFloat(gPointConstants.Get(j), light->GetLightSource().GetConstant());
					mesh->GetShader(i)->BindFloat(gPointLinears.Get(j), light->GetLightSource().GetLinear());
					mesh->GetShader(i)->BindFloat(gPointQuadratics.Get(j), light->GetLightSource().GetQuadratic());
				}

				for(int64_t j = 0; j < spotLightsBuffer.Size() && j < maxLightSources; j++){
					const auto* light = spotLightsBuffer[j];
					GADGET_BASIC_ASSERT(light != nullptr && light->GetParent() != nullptr);
					mesh->GetShader(i)->BindVector3(gSpotPositions.Get(j), light->GetParent()->GetPosition());
					mesh->GetShader(i)->BindVector3(gSpotDirections.Get(j), light->GetLightSource().GetDirection());
					mesh->GetShader(i)->BindFloat(gSpotCutOffs.Get(j), light->GetLightSource().GetCutOff());
					mesh->GetShader(i)->BindFloat(gSpotOuterCutOffs.Get(j), light->GetLightSource().GetOuterCutOff());
					mesh->GetShader(i)->BindColor(gSpotLightColors.Get(j), light->GetLightSource().GetColor());
					mesh->GetShader(i)->BindFloat(gSpotConstants.Get(j), light->GetLightSource().GetConstant());
					mesh->GetShader(i)->BindFloat(gSpotLinears.Get(j), light->GetLightSource().GetLinear());
					mesh->GetShader(i)->BindFloat(gSpotQuadratics.Get(j), light->GetLightSource().GetQuadratic());
				}

				for(int64_t j = 0; j < dirLightsBuffer.Size() && j < maxLightSources; j++){
					const auto* light = dirLightsBuffer[j];
					GADGET_BASIC_ASSERT(light != nullptr);
					mesh->GetShader(i)->BindVector3(gDirDirections.Get(j), light->GetLightSource().GetDirection());
					mesh->GetShader(i)->BindColor(gDirLightColors.Get(j), light->GetLightSource().GetColor());
				}

				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetMeshNumIndices(i)), GL_UNSIGNED_INT, nullptr);

				mesh->Unbind(i);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------
		//Animated meshes
		for(const auto& aMesh : animRendersBuffer){
			const Array<Matrix4>& skeletonInstance = aMesh->GetCurrentSkeletonInstance();

			for(size_t i = 0; i < aMesh->GetNumSubmeshes(); i++){
				aMesh->Bind(i);
				
				for(int32_t j = 0; j < skeletonInstance.Size(); j++){
					aMesh->GetShader(i)->BindMatrix4(gBoneIDs.Get(j), skeletonInstance[j]);
				}

				aMesh->GetShader(i)->BindMatrix4(SID("projectionMatrix"), proj);
				aMesh->GetShader(i)->BindMatrix4(SID("viewMatrix"), view);

				const Matrix4 modelMatrix = aMesh->GetParent()->GetTransformMatrix();
				aMesh->GetShader(i)->BindMatrix4(SID("modelMatrix"), modelMatrix);

				aMesh->GetShader(i)->BindMatrix3(SID("normalMatrix"), (modelMatrix.Inverse()).Transpose().ToMatrix3());

				aMesh->GetShader(i)->BindVector3(SID("viewPos"), cam->GetParent()->GetPosition());

				aMesh->GetShader(i)->BindInt(SID("numPointLights"), static_cast<int>(pointLightsBuffer.Size()));
				aMesh->GetShader(i)->BindInt(SID("numSpotLights"), static_cast<int>(spotLightsBuffer.Size()));
				aMesh->GetShader(i)->BindInt(SID("numDirLights"), static_cast<int>(dirLightsBuffer.Size()));

				for(int64_t j = 0; j < pointLightsBuffer.Size() && j < maxLightSources; j++){
					const auto* light = pointLightsBuffer[j];
					GADGET_BASIC_ASSERT(light != nullptr && light->GetParent() != nullptr);
					aMesh->GetShader(i)->BindVector3(gPointPositions.Get(j),	light->GetParent()->GetPosition());
					aMesh->GetShader(i)->BindColor(gPointLightColors.Get(j),	light->GetLightSource().GetColor());
					aMesh->GetShader(i)->BindFloat(gPointConstants.Get(j),		light->GetLightSource().GetConstant());
					aMesh->GetShader(i)->BindFloat(gPointLinears.Get(j),		light->GetLightSource().GetLinear());
					aMesh->GetShader(i)->BindFloat(gPointQuadratics.Get(j),		light->GetLightSource().GetQuadratic());
				}

				for(int64_t j = 0; j < spotLightsBuffer.Size() && j < maxLightSources; j++){
					const auto* light = spotLightsBuffer[j];
					GADGET_BASIC_ASSERT(light != nullptr && light->GetParent() != nullptr);
					aMesh->GetShader(i)->BindVector3(gSpotPositions.Get(j),		light->GetParent()->GetPosition());
					aMesh->GetShader(i)->BindVector3(gSpotDirections.Get(j),	light->GetLightSource().GetDirection());
					aMesh->GetShader(i)->BindFloat(gSpotCutOffs.Get(j),			light->GetLightSource().GetCutOff());
					aMesh->GetShader(i)->BindFloat(gSpotOuterCutOffs.Get(j),	light->GetLightSource().GetOuterCutOff());
					aMesh->GetShader(i)->BindColor(gSpotLightColors.Get(j),		light->GetLightSource().GetColor());
					aMesh->GetShader(i)->BindFloat(gSpotConstants.Get(j),		light->GetLightSource().GetConstant());
					aMesh->GetShader(i)->BindFloat(gSpotLinears.Get(j),			light->GetLightSource().GetLinear());
					aMesh->GetShader(i)->BindFloat(gSpotQuadratics.Get(j),		light->GetLightSource().GetQuadratic());
				}

				for(int64_t j = 0; j < dirLightsBuffer.Size() && j < maxLightSources; j++){
					const auto* light = dirLightsBuffer[j];
					GADGET_BASIC_ASSERT(light != nullptr);
					aMesh->GetShader(i)->BindVector3(gDirDirections.Get(j), light->GetLightSource().GetDirection());
					aMesh->GetShader(i)->BindColor(gDirLightColors.Get(j), light->GetLightSource().GetColor());
				}

				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(aMesh->GetMeshNumIndices(i)), GL_UNSIGNED_INT, nullptr);

				aMesh->Unbind(i);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------
		//Render UI elements

		Matrix4 uiProjection = Matrix4::Orthographic(-1.0f, 1.0f, -1.0f, 1.0f);
		uiProjection[0] = 1.0f / GetAspectRatio();

		//TEXT
		for(const auto& text : guiTextsBuffer){
			GADGET_BASIC_ASSERT(text != nullptr);
			if(text == nullptr || !text->IsActive() || text->GetText().empty()){
				continue;
			}

			text->GetTextMesh().GetShader()->Bind();
			text->GetTextMesh().GetShader()->BindMatrix4(SID("projectionMatrix"), uiProjection);
			text->GetTextMesh().GetShader()->BindColor(SID("textColor"), text->GetColor());

			text->GetTextMesh().GetMeshInfo()->Bind();

			const size_t totalWidthInPixels = text->GetTextMesh().GetTotalWidthInPixels();
			const size_t totalHeightInPixels = text->GetTextMesh().GetTotalHeightInPixels();

			GADGET_BASIC_ASSERT(totalWidthInPixels > 0);
			const float screenWidthPerPixel = text->GetSize().x / static_cast<float>(totalWidthInPixels);
			const float screenHeightPerPixel = screenWidthPerPixel;
			const float heightToUse = screenWidthPerPixel * static_cast<float>(totalHeightInPixels);

			float x = text->GetPosition().x - (text->GetSize().x / 2.0f);
			const float y = text->GetPosition().y - (heightToUse / 2.0f);

			GL_FontInfo* fontInfo = dynamic_cast<GL_FontInfo*>(text->GetTextMesh().GetFont()->GetFontInfo());
			GL_DynamicMeshInfo* meshInfo = dynamic_cast<GL_DynamicMeshInfo*>(text->GetTextMesh().GetMeshInfo());

			for(const char c : text->GetText()){
				const FreetypeFontCharacter& ch = text->GetTextMesh().GetFont()->GetCharacters().at(c);

				const GLfloat xpos = x + static_cast<float>(ch.left) * screenWidthPerPixel;
				const GLfloat ypos = y - static_cast<float>(ch.rows - ch.top) * screenHeightPerPixel;

				const GLfloat w = static_cast<float>(ch.width) * screenWidthPerPixel;
				const GLfloat h = static_cast<float>(ch.rows) * screenHeightPerPixel;
				// Update VBO for each character
				const StaticArray2D<GLfloat, 6, 4> vertices = {{
					{{ xpos,		ypos + h,	0.0f, 0.0f }},
					{{ xpos,		ypos,		0.0f, 1.0f }},
					{{ xpos + w,	ypos,		1.0f, 1.0f }},

					{{ xpos,		ypos + h,	0.0f, 0.0f }},
					{{ xpos + w,	ypos,		1.0f, 1.0f }},
					{{ xpos + w,	ypos + h,	1.0f, 0.0f }}
				}};

				//Don't bind the texture if it's a space, for some reason doing so causes OpenGL warnings
				if(c != ' '){
					//Render glyph texture over quad
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, fontInfo->GetTextureForChar(c));
				}

				//Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, meshInfo->GetVBO());
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices[0]);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				//Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				//Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += static_cast<float>(ch.advanceX >> 6) * screenWidthPerPixel; //Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels)) 
			}

			text->GetTextMesh().GetMeshInfo()->Unbind();
			text->GetTextMesh().GetShader()->Unbind();
		}

		//TEXTURES
		for(const auto& texture : guiTexturesBuffer){
			if(texture == nullptr || !texture->IsActive()){
				continue;
			}

			Shader* shader = texture->GetShader();
			shader->Bind();
			shader->BindMatrix4(SID("projectionMatrix"), uiProjection);
			shader->BindColor(SID("tintColor"), texture->GetTintColor());

			GL_DynamicMeshInfo* meshInfo = dynamic_cast<GL_DynamicMeshInfo*>(texture->GetMeshInfo());
			meshInfo->Bind();

			const float x = texture->GetPosition().x;
			const float y = texture->GetPosition().y;
			const float w = texture->GetSize().x;
			const float h = texture->GetSize().y;

			const StaticArray2D<GLfloat, 6, 4> vertices = {{
				{{ x - w, y + h, 0.0f, 0.0f }},
				{{ x - w, y - h, 0.0f, 1.0f }},
				{{ x + w, y - h, 1.0f, 1.0f }},

				{{ x - w, y + h, 0.0f, 0.0f }},
				{{ x + w, y - h, 1.0f, 1.0f }},
				{{ x + w, y + h, 1.0f, 0.0f }}
			}};

			GL_TextureInfo* textureInfo = dynamic_cast<GL_TextureInfo*>(texture->GetTextureInfo());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureInfo->GetTexture());

			//Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, meshInfo->GetVBO());
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices[0]);
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
	window->SwapBuffers();

	//So we don't accidentally reuse these pointers later
	camerasBuffer.Clear();
	rendersBuffer.Clear();
	animRendersBuffer.Clear();
	pointLightsBuffer.Clear();
	spotLightsBuffer.Clear();
	dirLightsBuffer.Clear();
	guiTextsBuffer.Clear();
	guiTexturesBuffer.Clear();
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

	glViewport(static_cast<GLint>(static_cast<float>(window->GetWidth()) * rect_.x),
				static_cast<GLint>(static_cast<float>(window->GetHeight()) * rect_.y),
				static_cast<GLsizei>(static_cast<float>(window->GetWidth()) * rect_.w),
				static_cast<GLsizei>(static_cast<float>(window->GetHeight()) * rect_.h));
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
			Debug::Log(SID("[RENDER]"), "Unsupported winding order [" + std::to_string(static_cast<int>(currentWindingOrder)) + "]!", Debug::Error, __FILE__, __LINE__);
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
			Debug::Log(SID("[RENDER]"), "Unsupported cull face [" + std::to_string(static_cast<int>(currentCullFace)) + "]!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}

Shader* Win32_GL_Renderer::GenerateAPIShader(StringID shaderResource_){
	GADGET_BASIC_ASSERT(shaderResource_ != StringID::None);
	return App::GetResourceManager().LoadResource<GL_Shader>(shaderResource_); //TODO - This feels bad...
}

MaterialInfo* Win32_GL_Renderer::GenerateAPIMaterialInfo([[maybe_unused]] const std::vector<Color>& colors_){ GADGET_ASSERT_NOT_IMPLEMENTED; return nullptr; }

std::vector<MeshInfo*> Win32_GL_Renderer::GenerateAPIMeshInfos(const Mesh& mesh_){
	std::vector<MeshInfo*> meshInfos;
	meshInfos.reserve(mesh_.submeshes.size());
	for(const auto& sm : mesh_.submeshes){
		meshInfos.push_back(new GL_MeshInfo(sm));
	}

	return meshInfos;
}

MeshInfo* Win32_GL_Renderer::GenerateAPIDynamicMeshInfo(size_t numVertices_, size_t numIndices_){
	return new GL_DynamicMeshInfo(numVertices_, numIndices_);
}

std::vector<MeshInfo*> Win32_GL_Renderer::GenerateAPIAnimMeshInfos(const AnimMesh& animMesh_){
	std::vector<MeshInfo*> meshInfos;
	meshInfos.reserve(animMesh_.submeshes.Size());
	for(const auto& sm : animMesh_.submeshes){
		meshInfos.push_back(new GL_AnimMeshInfo(sm));
	}

	return meshInfos;
}

TextureInfo* Win32_GL_Renderer::GenerateAPITextureInfo(const Texture& texture_){
	return new GL_TextureInfo(texture_);
}

FontInfo* Win32_GL_Renderer::GenerateAPIFontInfo(const FreetypeFont& font_){
	return new GL_FontInfo(font_);
}

static constexpr GLuint gDriverStorageWarning = 131169; //The driver allocated storage for renderbuffer - This should not be considered a warning
static constexpr GLuint gBufferVideoMemWarning = 131185; //Buffer object will use VIDEO memory - Irrelevant

void __stdcall Win32_GL_Renderer::GLDebugCallback(GLenum source_, GLenum type_, GLuint id_, GLenum severity_, [[maybe_unused]] GLsizei length_, const GLchar* message_, [[maybe_unused]] const void* useParam_){
	//Suppress useless messages
	switch(id_){
		case gDriverStorageWarning:
		case gBufferVideoMemWarning:
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
		case GL_DEBUG_SOURCE_OTHER: [[fallthrough]];
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
		case GL_DEBUG_TYPE_OTHER: [[fallthrough]];
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
		case GL_DEBUG_SEVERITY_LOW:
			Debug::Log(SID("OPENGL"), finalMessage, Debug::Warning);
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: [[fallthrough]];
		default:
			Debug::Log(SID("OPENGL"), finalMessage, Debug::Verbose);
			break;
	}
}