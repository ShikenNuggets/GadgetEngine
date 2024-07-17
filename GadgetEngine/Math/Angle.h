#ifndef GADGET_ANGLE_H
#define GADGET_ANGLE_H

namespace Gadget{
	//Forward declarations
	class Degree;
	class Radian;

	typedef Degree Angle; //Changing this will break everything but will allow you to use Radians by default if you desire

	class Degree{
	public:
		constexpr Degree(float angle_ = 0.0f) : angle(angle_){}
		explicit Degree(const Radian& r_);

		inline constexpr Degree& operator =(float a_){
			*this = Degree(a_);
			return *this;
		}

		Degree& operator =(Radian& r_);

		Radian ToRadians() const;
		float ToRadiansValue() const;

		inline constexpr float Get() const{ return angle; }
		inline constexpr operator float() const{ return Get(); }

		inline constexpr Degree operator -() const{ return Degree(-angle); }
		inline constexpr Degree operator +(float a_) const{ return angle + a_; }
		inline constexpr Degree operator +(const Degree& d_) const{ return angle + d_.Get(); }
		inline constexpr Degree operator -(float a_) const{ return angle - a_; }
		inline constexpr Degree operator -(const Degree& d_) const{ return angle - d_.Get(); }
		inline constexpr Degree operator *(float a_) const{ return angle * a_; }
		inline constexpr Degree operator *(const Degree& d_) const{ return angle * d_.Get(); }
		Degree operator /(float a_) const;
		Degree operator /(const Degree& d_) const;

		inline friend constexpr Degree operator +(float a_, const Degree& d_){ return d_ + a_; }
		inline friend constexpr Degree operator -(float a_, const Degree& d_){ return d_ - a_; }
		inline friend constexpr Degree operator *(float a_, const Degree& d_){ return d_ * a_; }
		inline friend Degree operator /(float a_, const Degree& d_){ return d_ / a_; }
		inline constexpr void operator +=(float a_){ *this = *this + a_; }
		inline constexpr void operator +=(const Degree& d_){ *this = *this + d_; }
		inline constexpr void operator -=(float a_){ *this = *this - a_; }
		inline constexpr void operator -=(const Degree& d_){ *this = *this + d_; }
		inline constexpr void operator *=(float a_){ *this = *this * a_; }
		inline constexpr void operator *=(const Degree& d_){ *this = *this * d_; }
		void operator /=(float a_);
		void operator /=(const Degree& d_);

		inline constexpr bool operator ==(float a_) const{ return angle == a_; }
		inline constexpr bool operator ==(const Degree& d_) const{ return angle == d_.Get(); }
		inline constexpr bool operator !=(float a_) const{ return angle != a_; }
		inline constexpr bool operator !=(const Degree& d_) const{ return angle != d_.Get(); }
		inline constexpr bool operator >(float a_) const{ return angle > a_; }
		inline constexpr bool operator >(const Degree& d_) const{ return angle > d_.Get(); }
		inline constexpr bool operator <(float a_) const{ return angle < a_; }
		inline constexpr bool operator <(const Degree& d_) const{ return angle < d_.Get(); }
		inline constexpr bool operator >=(float a_) const{ return angle >= a_; }
		inline constexpr bool operator >=(const Degree& d_) const{ return angle >= d_.Get(); }
		inline constexpr bool operator <=(float a_) const{ return angle <= a_; }
		inline constexpr bool operator <=(const Degree& d_) const{ return angle <= d_.Get(); }

		inline friend constexpr bool operator ==(float a_, const Degree& d_)	{ return d_ == a_; }
		inline friend constexpr bool operator !=(float a_, const Degree& d_)	{ return d_ != a_; }
		inline friend constexpr bool operator >(float a_, const Degree& d_)		{ return d_ > a_; }
		inline friend constexpr bool operator <(float a_, const Degree& d_)		{ return d_ < a_; }
		inline friend constexpr bool operator >=(float a_, const Degree& d_)	{ return d_ >= a_; }
		inline friend constexpr bool operator <=(float a_, const Degree& d_)	{ return d_ <= a_; }

		bool IsValid() const;

	private:
		float angle;
	};

	class Radian{
	public:
		constexpr Radian(float angle_ = 0.0f) : angle(angle_){}
		explicit Radian(const Degree& d);

		inline constexpr Radian& operator =(float a_){
			*this = Radian(a_);
			return *this;
		}
		
		Radian& operator =(Degree& r_);

		Degree ToDegrees() const;
		float ToDegreesValue() const;

		inline constexpr float Get() const{ return angle; }
		inline constexpr operator float() const{ return Get(); }

		inline constexpr Radian operator -() const{ return Radian(-angle); }
		inline constexpr Radian operator +(float a_) const{ return angle + a_; }
		inline constexpr Radian operator +(const Radian& r_) const{ return angle + r_.Get(); }
		inline constexpr Radian operator -(float a_) const{ return angle - a_; }
		inline constexpr Radian operator -(const Radian& r_) const{ return angle - r_.Get(); }
		inline constexpr Radian operator *(float a_) const{ return angle * a_; }
		inline constexpr Radian operator *(const Radian& r_) const{ return angle * r_.Get(); }
		Radian operator /(float a_) const;
		Radian operator /(const Radian& r_) const;

		inline friend constexpr Radian operator +(float a_, const Radian& d_){ return d_ + a_; }
		inline friend constexpr Radian operator -(float a_, const Radian& d_){ return d_ - a_; }
		inline friend constexpr Radian operator *(float a_, const Radian& d_){ return d_ * a_; }
		inline friend Radian operator /(float a_, const Radian& d_){ return d_ / a_; }

		inline constexpr void operator +=(float a_){ angle += a_; }
		inline constexpr void operator +=(const Radian& r_){ angle += r_.Get(); }
		inline constexpr void operator -=(float a_){ angle -= a_; }
		inline constexpr void operator -=(const Radian& r_){ angle -= r_.Get(); }
		inline constexpr void operator *=(float a_){ angle *= a_; }
		inline constexpr void operator *=(const Radian& r_){ angle *= r_.Get(); }
		void operator /=(float a_);
		void operator /=(const Radian& r_);

		inline constexpr bool operator ==(float a_) const{ return angle == a_; }
		inline constexpr bool operator ==(const Radian& r_) const{ return angle == r_.Get(); }
		inline constexpr bool operator !=(float a_) const{ return angle != a_; }
		inline constexpr bool operator !=(const Radian& r_) const{ return angle != r_.Get(); }
		inline constexpr bool operator >(float a_) const{ return angle > a_; }
		inline constexpr bool operator >(const Radian& r_) const{ return angle > r_.Get(); }
		inline constexpr bool operator <(float a_) const{ return angle < a_; }
		inline constexpr bool operator <(const Radian& r_) const{ return angle < r_.Get(); }
		inline constexpr bool operator >=(float a_) const{ return angle >= a_; }
		inline constexpr bool operator >=(const Radian& r_) const{ return angle >= r_.Get(); }
		inline constexpr bool operator <=(float a_) const{ return angle <= a_; }
		inline constexpr bool operator <=(const Radian& r_) const{ return angle <= r_.Get(); }

		inline friend constexpr bool operator ==(float a_, const Radian& d_)	{ return d_ == a_; }
		inline friend constexpr bool operator !=(float a_, const Radian& d_)	{ return d_ != a_; }
		inline friend constexpr bool operator >(float a_, const Radian& d_)		{ return d_ > a_; }
		inline friend constexpr bool operator <(float a_, const Radian& d_)		{ return d_ < a_; }
		inline friend constexpr bool operator >=(float a_, const Radian& d_)	{ return d_ >= a_; }
		inline friend constexpr bool operator <=(float a_, const Radian& d_)	{ return d_ <= a_; }

		bool IsValid() const;

	private:
		float angle;
	};
}

#endif //!GADGET_ANGLE_H