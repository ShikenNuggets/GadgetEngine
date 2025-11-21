#ifndef GADGET_SKYBOX_COMPONENT_H
#define GADGET_SKYBOX_COMPONENT_H

#include "Game/SceneComponent.h"
#include "Graphics/OpenGL/GL_Shader.h"
#include "Graphics/OpenGL/GL_CubemapInfo.h"

namespace Gadget{
	class SkyboxComponent : public SceneComponent{
	public:
		SkyboxComponent(Scene* parent_, StringID skyboxName_, StringID shader_ = SID("SkyboxShader"));
		virtual ~SkyboxComponent();

		void Bind(const Matrix4& proj_, const Matrix4& view_);
		void Unbind();

	private:
		StringID shaderName;
		Shader* shader;
		GL_CubemapInfo* cubemapInfo;
	};
}

#endif //!GADGET_SKYBOX_COMPONENT_H