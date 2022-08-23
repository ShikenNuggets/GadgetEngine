#ifndef GADGET_SHADER_H
#define GADGET_SHADER_H

namespace Gadget{
	class Shader{
	public:
		Shader(){}
		virtual ~Shader(){}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}

#endif //!GADGET_SHADER_H