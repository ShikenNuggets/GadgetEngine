#ifndef GADGET_SHADER_H
#define GADGET_SHADER_H

#include "Graphics/Color.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Resource/Resource.h"
#include "Utils/StringID.h"

namespace Gadget{
	class Shader : public Resource{
	public:
		Shader(){}
		virtual ~Shader(){}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void BindInt(StringID uniformName_, int value_) = 0;
		virtual void BindFloat(StringID uniformName_, float value_) = 0;
		virtual void BindVector2(StringID uniformName_, const Vector2& vec_) = 0;
		virtual void BindVector3(StringID uniformName_, const Vector3& vec_) = 0;
		virtual void BindVector4(StringID uniformName_, const Vector4& vec_) = 0;
		virtual void BindMatrix3(StringID uniformName_, const Matrix3& mat3_) = 0;
		virtual void BindMatrix4(StringID uniformName_, const Matrix4& mat4_) = 0;
		virtual void BindColor(StringID uniformName_, const Color& color_) = 0;
	};
}

#endif //!GADGET_SHADER_H