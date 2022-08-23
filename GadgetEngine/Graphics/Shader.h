#ifndef GADGET_SHADER_H
#define GADGET_SHADER_H

#include "Resource/Resource.h"

namespace Gadget{
	class Shader : public Resource{
	public:
		Shader(){}
		virtual ~Shader(){}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}

#endif //!GADGET_SHADER_H