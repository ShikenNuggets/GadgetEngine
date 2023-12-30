#ifndef GADGET_NAMED_VAR_H
#define GADGET_NAMED_VAR_H

#include "Var.h"

namespace Gadget{
	class NamedVar{
	public:
		explicit constexpr NamedVar(StringID name_, std::nullptr_t value_) : name(name_), value(value_){}
		constexpr NamedVar(StringID name_, Var value_) : name(name_), value(value_){}
		constexpr NamedVar(StringID name_, StringID value_) : name(name_), value(value_){}
		constexpr NamedVar(StringID name_, bool value_) : name(name_), value(value_){}
		constexpr NamedVar(StringID name_, int32_t value_) : name(name_), value(value_){}
		constexpr NamedVar(StringID name_, int64_t value_) : name(name_), value(value_){}
		constexpr NamedVar(StringID name_, uint32_t value_) : name(name_), value(value_){}
		constexpr NamedVar(StringID name_, uint64_t value_) : name(name_), value(value_){}
		constexpr NamedVar(StringID name_, double value_) : name(name_), value(value_){}
		explicit constexpr NamedVar(std::nullptr_t value_) : name(StringID::None), value(value_){}

		constexpr void operator=(const Var& var_){ value = var_; }

		constexpr StringID Name() const{ return name; }
		constexpr Var Value() const{ return value; }

		StringID ToStr() const{ return value.ToStr(); }
		bool ToBool() const{ return value.ToBool(); }
		double ToNumber() const{ return value.ToNumber(); }

		template <class T>
		T ToNumber() const{ return value.ToNumber<T>(); }

	private:
		StringID name;
		Var value;
	};

	class NamedVarList{
	public:
		constexpr NamedVarList(StringID name_, const std::vector<NamedVar>& values_) : name(name_), values(values_){}

		constexpr StringID Name() const{ return name; }
		constexpr const std::vector<NamedVar>& Value() const{ return values; }
		constexpr size_t Size() const{ return values.size(); }

		constexpr bool HasValue(StringID name_) const{
			for(const auto& nv : values){
				if(nv.Name() == name_){
					return true;
				}
			}

			return false;
		}

		constexpr NamedVar GetValue(StringID name_) const{
			GADGET_BASIC_ASSERT(HasValue(name_));

			for(const auto& nv : values){
				if(nv.Name() == name_){
					return nv;
				}
			}

			return NamedVar(nullptr);
		}

		constexpr NamedVar GetValue(size_t index_) const{
			GADGET_BASIC_ASSERT(index_ < Size());
			if(index_ < Size()){
				return NamedVar(nullptr);
			}

			return values[index_];
		}

	private:
		StringID name;
		std::vector<NamedVar> values;
	};
}

#endif //!GADGET_NAMED_VAR_H