#ifndef GADGET_MATERIAL_H
#define GADGET_MATERIAL_H

#include "Graphics/Shader.h"

namespace Gadget{
	class Material{
	public:
		Material(StringID shaderResource_);
		virtual ~Material();

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		inline Shader* GetShader() const{ return shader; }

	protected:
		StringID shaderResourceName;
		Shader* shader;
	};
}

#endif //!GADGET_MATERIAL_H