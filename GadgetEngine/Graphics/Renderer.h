#ifndef GADGET_RENDERER_H
#define GADGET_RENDERER_H

#include <memory>

#include "Color.h"
#include "FontInfo.h"
#include "MeshInfo.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureInfo.h"
#include "ViewportRect.h"
#include "Window.h"
#include "Graphics/Text/FreetypeFont.h"
#include "Math/MathObjects.h"
#include "Graphics/Materials/MaterialInfo.h"

namespace Gadget{
	//Forward Declarations
	class Scene;

	class Renderer{
	public:
		enum class API : uint8_t{
			None = 0,
			OpenGL,
			DX12,

			API_MAX //Do not put any values below this
		};

		enum class WindingOrder : uint8_t{
			Clockwise,
			CounterClockwise,

			WindingOrder_MAX //Do not put any values below this
		};

		enum class CullFace : uint8_t{
			None,
			Back,
			Front,
			All,

			CullFace_MAX //Do not put any values below this
		};

		Renderer(API renderAPI_, WindingOrder order_ = WindingOrder::CounterClockwise, CullFace cullface_ = CullFace::Back) : renderAPI(renderAPI_), postInitComplete(false), window(nullptr), currentWindingOrder(order_), currentCullFace(cullface_){
			GADGET_BASIC_ASSERT(renderAPI_ < API::API_MAX);
			GADGET_BASIC_ASSERT(order_ < WindingOrder::WindingOrder_MAX);
			GADGET_BASIC_ASSERT(cullface_ < CullFace::CullFace_MAX);
		}

		virtual ~Renderer(){}

		virtual void PostInit(){ postInitComplete = true; }

		API GetRenderAPI() const{ return renderAPI; }
		std::weak_ptr<Window> GetWindow() const{ return window; }

		virtual void Render(const Scene* scene_) = 0;
		virtual void ClearScreen() = 0;
		virtual void SetClearColor(const Color& color_) = 0;
		virtual void SetViewportRect(const Rect& rect_) = 0;

		virtual void HandleWindowEvents(){
			if(window != nullptr){
				window->HandleEvents();
			}
		}

		virtual void OnResize(int newWidth_, int newHeight_) = 0; //TODO - Update all camera projection matrices

		virtual void SetWindingOrder(WindingOrder order_){
			GADGET_BASIC_ASSERT(order_ < WindingOrder::WindingOrder_MAX);
			currentWindingOrder = order_;
		}

		virtual void SetCullFace(CullFace cullFace_){
			GADGET_BASIC_ASSERT(cullFace_ < CullFace::CullFace_MAX);
			currentCullFace = cullFace_;
		}

		float GetRefreshRate(){
			if(window != nullptr){
				return window->GetRefreshRate();
			}

			Debug::Log(SID("RENDER"), "Renderer::GetRefreshRate called, but there was no window", Debug::Warning, __FILE__, __LINE__);
			return 0.0f;
		}

		float GetAspectRatio(){
			if(window != nullptr){
				return static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());
			}

			Debug::Log(SID("RENDER"), "Renderer::GetAspectRatio called, but there was no window", Debug::Warning, __FILE__, __LINE__);
			return 0.0f;
		}

		void ResetViewportRect(){ SetViewportRect(ViewportRect::Fullscreen); }

		virtual Shader* GenerateAPIShader(StringID shaderResource_) = 0;
		virtual MaterialInfo* GenerateAPIMaterialInfo(const std::vector<Color>& colors_) = 0;
		virtual MeshInfo* GenerateAPIMeshInfo(const Mesh& mesh_) = 0;
		virtual MeshInfo* GenerateAPIDynamicMeshInfo(size_t numVertices, size_t numIndices) = 0;
		virtual TextureInfo* GenerateAPITextureInfo(const Texture& texture_) = 0;
		virtual FontInfo* GenerateAPIFontInfo(const FreetypeFont& font_) = 0;

	protected:
		const API renderAPI;
		bool postInitComplete;
		std::shared_ptr<Window> window;
		WindingOrder currentWindingOrder;
		CullFace currentCullFace;
	};
}

#endif //!GADGET_RENDERER_H