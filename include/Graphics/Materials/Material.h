#ifndef GADGET_MATERIAL_H
#define GADGET_MATERIAL_H

#include "Graphics/Shader.h"
#include "Utils/NamedVar.h"

namespace Gadget{
	class Material{
	public:
		Material(StringID shaderResource_);
		Material(const NamedVarList& varList_);
		virtual ~Material();

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual bool HasLighting() const = 0;

		inline StringID GetShaderName() const{ return shaderResourceName; }

		inline Shader* GetShader(){
			if(shader == nullptr){
				LoadShader();
			}

			return shader;
		}

		virtual StringID Type() const = 0;

		void InvalidateShader();

		virtual void InvalidateAllAPIInfos(){
			InvalidateShader();
		}

		virtual void Serialize(NamedVarList& varList_) const;

	protected:
		StringID shaderResourceName;
		Shader* shader;

		void LoadShader();
		virtual void Deserialize(const NamedVarList& varList_);
	};
}

#endif //!GADGET_MATERIAL_H