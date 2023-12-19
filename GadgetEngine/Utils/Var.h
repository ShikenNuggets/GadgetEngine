#ifndef GADGET_VAR_H
#define GADGET_VAR_H

#include "Debug.h"
#include "StringID.h"

namespace Gadget{
	class Var{
	public:
		enum class Type{
			Null,
			String,
			Bool,
			Number
		};

		explicit constexpr Var(std::nullptr_t) : type(Type::Null), boolVal(false){}
		constexpr Var(StringID value_) : type(Type::String), strValue(value_){}
		Var(const std::string& value_) : type(Type::String), strValue(StringID::ProcessString(value_)){}
		constexpr Var(bool value_) : type(Type::Bool), boolVal(value_){}
		constexpr Var(int32_t value_) : type(Type::Number), numVal(static_cast<double>(value_)){}
		constexpr Var(int64_t value_) : type(Type::Number), numVal(static_cast<double>(value_)){}
		constexpr Var(uint32_t value_) : type(Type::Number), numVal(static_cast<double>(value_)){}
		constexpr Var(uint64_t value_) : type(Type::Number), numVal(static_cast<double>(value_)){}
		constexpr Var(double value_) : type(Type::Number), numVal(value_){}

		constexpr void operator=(const Var& var_){
			type = var_.type;
			switch(type){
				case Type::Null:
					boolVal = false;
					break;
				case Type::String:
					strValue = var_.strValue;
					break;
				case Type::Bool:
					boolVal = var_.boolVal;
					break;
				case Type::Number:
					numVal = var_.numVal;
					break;
				default:
					GADGET_ASSERT(false, "Unhandled Var type!");
					break;
			}
		}

		constexpr bool operator==(std::nullptr_t){ return type == Type::Null; }
		constexpr bool operator==(StringID value_){ return type == Type::String && strValue == value_; }
		constexpr bool operator==(const std::string& value_){ return type == Type::String && strValue.GetString() == value_; }
		constexpr bool operator==(bool value_){ return type == Type::Bool && boolVal == value_; }
		constexpr bool operator==(int32_t value_){ return type == Type::Number && numVal == static_cast<double>(value_); }
		constexpr bool operator==(int64_t value_){ return type == Type::Number && numVal == static_cast<double>(value_); }
		constexpr bool operator==(uint32_t value_){ return type == Type::Number && numVal == static_cast<double>(value_); }
		constexpr bool operator==(uint64_t value_){ return type == Type::Number && numVal == static_cast<double>(value_); }
		constexpr bool operator==(double value_){ return type == Type::Number && numVal == static_cast<double>(value_); }

		constexpr bool operator==(const Var& var_){
			switch(type){
				case Type::Null:
					return *this == nullptr;
				case Type::String:
					return *this == var_.ToStr();
				case Type::Bool:
					return *this == var_.ToBool();
				case Type::Number:
					return *this == var_.ToNumber();
				default:
					GADGET_ASSERT(false, "Unhandled Var Type in operator==");
					break;
			}

			return false;
		}

		constexpr bool operator!=(std::nullptr_t){ return type == Type::Null; }
		constexpr bool operator!=(StringID value_){ return type == Type::String && strValue != value_; }
		constexpr bool operator!=(const std::string& value_){ return type == Type::String && strValue.GetString() != value_; }
		constexpr bool operator!=(bool value_){ return type == Type::Bool && boolVal != value_; }
		constexpr bool operator!=(int32_t value_){ return type == Type::Number && numVal != static_cast<double>(value_); }
		constexpr bool operator!=(int64_t value_){ return type == Type::Number && numVal != static_cast<double>(value_); }
		constexpr bool operator!=(uint32_t value_){ return type == Type::Number && numVal != static_cast<double>(value_); }
		constexpr bool operator!=(uint64_t value_){ return type == Type::Number && numVal != static_cast<double>(value_); }
		constexpr bool operator!=(double value_){ return type == Type::Number && numVal != static_cast<double>(value_); }

		constexpr bool operator!=(const Var& var_){
			switch(type){
				case Type::Null:
					return *this != nullptr;
				case Type::String:
					return *this != var_.ToStr();
				case Type::Bool:
					return *this != var_.ToBool();
				case Type::Number:
					return *this != var_.ToNumber();
				default:
					GADGET_ASSERT(false, "Unhandled Var Type in operator!=");
					break;
			}

			return false;
		}

		Type GetType() const{ return type; }

		StringID ToStr() const{
			if(type == Type::String){
				return strValue;
			}

			return StringID::None;
		}

		bool ToBool() const{
			if(type == Type::Bool){
				return boolVal;
			}

			return false;
		}

		double ToNumber() const{
			if(type == Type::Number){
				return numVal;
			}

			return 0.0;
		}

		void SetValue(StringID value_){
			strValue = value_;
			type = Type::String;
		}

		void SetValue(bool value_){
			boolVal = value_;
			type = Type::Bool;
		}

		void SetValue(double value_){
			numVal = value_;
			type = Type::Number;
		}

		void SetValue(int32_t value_){ SetValue(static_cast<double>(value_)); }
		void SetValue(int64_t value_){ SetValue(static_cast<double>(value_)); }
		void SetValue(uint32_t value_){ SetValue(static_cast<double>(value_)); }
		void SetValue(uint64_t value_){ SetValue(static_cast<double>(value_)); }

		static Var StringToVar(const std::string& str_){
			std::string upper = Utils::ToUpper(str_);
			if(upper == "NULL"){
				return Var(nullptr);
			}else if(upper == "TRUE"){
				return Var(true);
			}else if(upper == "FALSE"){
				return Var(false);
			}else if(Utils::StringIsNumber(str_)){
				return Var(Utils::StringToNumber(str_));
			}else{
				return Var(StringID::ProcessString(str_));
			}
		}

	private:
		Type type;
		union{
			StringID strValue;
			bool boolVal;
			double numVal;
		};
	};
}

#endif //!GADGET_VAR_H