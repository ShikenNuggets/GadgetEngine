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

		constexpr void operator=(const Var& var_){ value = var_; }

		constexpr StringID Name() const{ return name; }
		constexpr Var Value() const{ return value; }

		StringID ToStr() const{ return value.ToStr(); }
		bool ToBool() const{ return value.ToBool(); }
		double ToNumber() const{ return value.ToNumber(); }

	private:
		StringID name;
		Var value;
	};

	class NamedVarList{
	public:
		constexpr NamedVarList(StringID name_, const std::vector<Var>& values_) : name(name_), values(values_){}

		constexpr NamedVarList(StringID name_, const std::vector<StringID>& values_) : name(name_), values(){
			for(const auto& v : values_){
				values.push_back(Var(v));
			}
		}

		constexpr NamedVarList(StringID name_, const std::vector<std::string>& values_) : name(name_), values(){
			for(const auto& v : values_){
				values.push_back(Var(v));
			}
		}

		constexpr StringID Name() const{ return name; }
		constexpr const std::vector<Var>& Value() const{ return values; }
		constexpr size_t Size() const{ return values.size(); }

		constexpr Var GetValue(size_t index_) const{
			GADGET_BASIC_ASSERT(index_ < Size());
			if(index_ < Size()){
				return Var(nullptr);
			}

			return values[index_];
		}

	private:
		StringID name;
		std::vector<Var> values;
	};
}

#endif //!GADGET_NAMED_VAR_H