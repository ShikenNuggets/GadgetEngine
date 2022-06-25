#ifndef GADGET_ANGLE_H
#define GADGET_ANGLE_H

#include "Math.h"

namespace Gadget{
	//Forward declarations
	class Degree;
	class Radian;
	class Angle;

	class Degree{
	public:
		Degree(float angle_);
		explicit Degree(const Radian& r_);
		Degree& operator =(float a_);
		Degree& operator =(Radian& r_);

		Radian ToRadians() const;

		float GetAngle() const{ return angle; }

		Degree operator -() const;
		Degree operator +(float a_) const;
		Degree operator +(const Radian& r_) const;
		Degree operator +(const Degree& d_) const;
		Degree operator -(float a_) const;
		Degree operator -(const Radian& r_) const;
		Degree operator -(const Degree& d_) const;
		Degree operator *(float a_) const;
		Degree operator *(const Radian& r_) const;
		Degree operator *(const Degree& d_) const;
		Degree operator /(float a_) const;
		Degree operator /(const Radian& r_) const;
		Degree operator /(const Degree& d_) const;

		inline friend Degree operator +(float a_, const Degree& d_){ return d_ + a_; }
		inline friend Degree operator -(float a_, const Degree& d_){ return d_ - a_; }
		inline friend Degree operator *(float a_, const Degree& d_){ return d_ * a_; }
		inline friend Degree operator /(float a_, const Degree& d_){ return d_ / a_; }

		void operator +=(float a_);
		void operator +=(const Radian& r_);
		void operator +=(const Degree& d_);
		void operator -=(float a_);
		void operator -=(const Radian& r_);
		void operator -=(const Degree& d_);
		void operator *=(float a_);
		void operator *=(const Radian& r_);
		void operator *=(const Degree& d_);
		void operator /=(float a_);
		void operator /=(const Radian& r_);
		void operator /=(const Degree& d_);

		bool operator ==(float a_) const;
		bool operator ==(const Radian& r_) const;
		bool operator ==(const Degree& d_) const;
		bool operator !=(float a_) const;
		bool operator !=(const Radian& r_) const;
		bool operator !=(const Degree& d_) const;
		bool operator >(float a_) const;
		bool operator >(const Radian& r_) const;
		bool operator >(const Degree& d_) const;
		bool operator <(float a_) const;
		bool operator <(const Radian& r_) const;
		bool operator <(const Degree& d_) const;
		bool operator >=(float a_) const;
		bool operator >=(const Radian& r_) const;
		bool operator >=(const Degree& d_) const;
		bool operator <=(float a_) const;
		bool operator <=(const Radian& r_) const;
		bool operator <=(const Degree& d_) const;

		inline friend bool operator ==(float a_, const Degree& d_)	{ return d_ == a_; }
		inline friend bool operator !=(float a_, const Degree& d_)	{ return d_ != a_; }
		inline friend bool operator >(float a_, const Degree& d_)	{ return d_ > a_; }
		inline friend bool operator <(float a_, const Degree& d_)	{ return d_ < a_; }
		inline friend bool operator >=(float a_, const Degree& d_)	{ return d_ >= a_; }
		inline friend bool operator <=(float a_, const Degree& d_)	{ return d_ <= a_; }

	private:
		float angle;
	};

	class Radian{
	public:
		Radian(float angle_);
		explicit Radian(const Degree& d);
		Radian& operator =(float a_);
		Radian& operator =(Degree& r_);

		Degree ToDegrees() const;

		float GetAngle() const{ return angle; }

		Radian operator -() const;
		Radian operator +(float a_) const;
		Radian operator +(const Radian& r_) const;
		Radian operator +(const Degree& d_) const;
		Radian operator -(float a_) const;
		Radian operator -(const Radian& r_) const;
		Radian operator -(const Degree& d_) const;
		Radian operator *(float a_) const;
		Radian operator *(const Radian& r_) const;
		Radian operator *(const Degree& d_) const;
		Radian operator /(float a_) const;
		Radian operator /(const Radian& r_) const;
		Radian operator /(const Degree& d_) const;

		inline friend Radian operator +(float a_, const Radian& d_) { return d_ + a_; }
		inline friend Radian operator -(float a_, const Radian& d_) { return d_ - a_; }
		inline friend Radian operator *(float a_, const Radian& d_) { return d_ * a_; }
		inline friend Radian operator /(float a_, const Radian& d_) { return d_ / a_; }

		void operator +=(float a_);
		void operator +=(const Radian& r_);
		void operator +=(const Degree& d_);
		void operator -=(float a_);
		void operator -=(const Radian& r_);
		void operator -=(const Degree& d_);
		void operator *=(float a_);
		void operator *=(const Radian& r_);
		void operator *=(const Degree& d_);
		void operator /=(float a_);
		void operator /=(const Radian& r_);
		void operator /=(const Degree& d_);

		bool operator ==(float a_) const;
		bool operator ==(const Radian& r_) const;
		bool operator ==(const Degree& d_) const;
		bool operator !=(float a_) const;
		bool operator !=(const Radian& r_) const;
		bool operator !=(const Degree& d_) const;
		bool operator >(float a_) const;
		bool operator >(const Radian& r_) const;
		bool operator >(const Degree& d_) const;
		bool operator <(float a_) const;
		bool operator <(const Radian& r_) const;
		bool operator <(const Degree& d_) const;
		bool operator >=(float a_) const;
		bool operator >=(const Radian& r_) const;
		bool operator >=(const Degree& d_) const;
		bool operator <=(float a_) const;
		bool operator <=(const Radian& r_) const;
		bool operator <=(const Degree& d_) const;

		inline friend bool operator ==(float a_, const Radian& d_)	{ return d_ == a_; }
		inline friend bool operator !=(float a_, const Radian& d_)	{ return d_ != a_; }
		inline friend bool operator >(float a_, const Radian& d_)	{ return d_ > a_; }
		inline friend bool operator <(float a_, const Radian& d_)	{ return d_ < a_; }
		inline friend bool operator >=(float a_, const Radian& d_)	{ return d_ >= a_; }
		inline friend bool operator <=(float a_, const Radian& d_)	{ return d_ <= a_; }

	private:
		float angle;
	};

	class Angle : Degree{
	public:
		Angle(float a);
		Angle(Degree d);
		Angle(Radian r);
	};
}

#endif //!GADGET_ANGLE_H