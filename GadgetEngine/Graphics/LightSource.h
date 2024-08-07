#ifndef GADGET_LIGHT_SOURCE_H
#define GADGET_LIGHT_SOURCE_H

#include "Color.h"
#include "Math/Vector.h"

namespace Gadget{
	class LightSource{
	public:
		constexpr LightSource(const Color& color_ = Color::White()) : color(color_), ambientIntensity(0.1f), diffuseIntensity(1.0f){}
		virtual ~LightSource(){}

		constexpr Color GetColor() const{ return color; }
		constexpr float GetAmbientIntensity() const{ return ambientIntensity; }
		constexpr float GetDiffuseIntensity() const{ return diffuseIntensity; }

		void SetColor(const Color& color_){
			GADGET_BASIC_ASSERT(color_.IsValid());
			color = color_;
		}

		void SetAmbientIntensity(float value_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(value_));
			ambientIntensity = value_;
		}
		
		void SetDiffuseIntensity(float value_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(value_));
			diffuseIntensity = value_;
		}

	protected:
		Color color;
		float ambientIntensity;
		float diffuseIntensity;
	};

	class PointLight : public LightSource{
	public:
		constexpr PointLight(const Color& color_ = Color::White()) : LightSource(color_), constant(1.0f), linear(0.09f), quadratic(0.032f){}

		constexpr float GetConstant() const{ return constant; }
		constexpr float GetLinear() const{ return linear; }
		constexpr float GetQuadratic() const{ return quadratic; }

	private:
		float constant;
		float linear;
		float quadratic;
	};

	class SpotLight : public LightSource{
	public:
		constexpr SpotLight(const Vector3& direction_, Degree cutOff_ = Degree(12.5f), Degree outerCutOff_ = Degree(15.0f), const Color& color_ = Color::White()) : LightSource(color_), direction(direction_), cutOff(cutOff_), outerCutOff(outerCutOff_), constant(1.0f), linear(0.09f), quadratic(0.032f){}

		constexpr Vector3 GetDirection() const{ return direction; }
		constexpr Degree GetCutOff() const{ return cutOff; }
		constexpr Degree GetOuterCutOff() const{ return outerCutOff; }
		constexpr float GetConstant() const{ return constant; }
		constexpr float GetLinear() const{ return linear; }
		constexpr float GetQuadratic() const{ return quadratic; }

		void SetDirection(const Vector3& dir_){
			GADGET_BASIC_ASSERT(dir_.IsValid());
			direction = dir_;
		}

		void SetCutOff(float cutOff_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(cutOff_));
			cutOff = cutOff_;
		}

		void SetOuterCutOff(float outerCutOff_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(outerCutOff_));
			outerCutOff = outerCutOff_;
		}

	private:
		Vector3 direction;
		Degree cutOff;
		Degree outerCutOff;
		float constant;
		float linear;
		float quadratic;
	};

	class DirectionalLight : public LightSource{
	public:
		constexpr DirectionalLight(const Vector3& direction_, const Color& color_ = Color::White()) : LightSource(color_), direction(direction_){}

		constexpr Vector3 GetDirection() const{ return direction; }

		void SetDirection(const Vector3& dir_){
			GADGET_BASIC_ASSERT(dir_.IsValid());
			direction = dir_;
		}

	private:
		Vector3 direction;
	};
}

#endif //!GADGET_LIGHT_SOURCE_H9