#ifndef GADGET_STRING_ID_H
#define GADGET_STRING_ID_H

#include <string>
#include <unordered_map>

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26495)	//Uninitialized member variable
#pragma warning(disable : 26800)	//Use of a moved object
#pragma warning(disable : 26819)	//Unnanotated fallthrough in switch statement
#include <nlohmann/json.hpp>
#pragma warning(default : 26495)
#pragma warning(default : 26800)
#pragma warning(default : 26819)

#include "Utils/Hash.h"

namespace Gadget{
	class StringID{
	public:
		static const StringID None;

		explicit constexpr StringID(uint64_t id_) : id(id_){}
		void operator =(StringID a_){ id = a_.id; }

		std::string GetString() const;
		uint64_t GetID() const{ return id; }

		constexpr inline bool operator ==(StringID a_) const{ return id == a_.id; }
		constexpr inline bool operator !=(StringID a_) const{ return id != a_.id; }
		constexpr inline bool operator >(StringID a_) const{ return id > a_.id; }
		constexpr inline bool operator <(StringID a_) const{ return id < a_.id; }
		constexpr inline bool operator >=(StringID a_) const{ return id >= a_.id; }
		constexpr inline bool operator <=(StringID a_) const{ return id <= a_.id; }

		//DON'T USE THIS DIRECTLY, USE THE SID() MACRO
		static StringID InternString(StringID sid_, const char* str_);
		static std::string GetStringFromID(StringID id_);
		static std::string GetStringFromID(uint64_t id_);

		//Only use this if the SID() macro cannot be used
		static inline StringID ProcessString(const std::string& str_){
			return InternString(StringID(Hash::MurmurHash64A(str_.c_str(), str_.length())), str_.c_str());
		}

		static inline std::vector<std::string> ToStringList(const std::vector<StringID>& stringIds_){
			std::vector<std::string> strings;
			for(const auto& sid : stringIds_){
				strings.push_back(sid.GetString());
			}

			return strings;
		}

		static inline std::vector<StringID> ToStringIDList(const std::vector<std::string>& strings_){
			std::vector<StringID> stringIds;
			for(const auto& sid : strings_){
				stringIds.push_back(ProcessString(sid));
			}

			return stringIds;
		}

	private:
		//This is a little silly, but it prevents a static initialization order fiasco
		static inline std::unordered_map<uint64_t, std::string>& StringIDTable(){
			static auto stringIdTable = std::unordered_map<uint64_t, std::string>();
			return stringIdTable;
		}

		uint64_t id;
	};
}

//User-defined literal - DON'T USE THIS DIRECTLY UNLESS YOU KNOW WHAT YOU'RE DOING, USE THE SID() MACRO
constexpr inline Gadget::StringID operator "" _sid(const char* str_, size_t len_){ return Gadget::StringID(Gadget::Hash::MurmurHash64A(str_, len_)); }

//Use this to create a hashed string ID and add it to the string database
#define SID(str) Gadget::StringID::InternString(str ""_sid, str)

//JSON Serializing/Deserializing
namespace Gadget{
	inline void to_json(nlohmann::json& j_, const StringID& s_){
		j_ = nlohmann::json{ {"string", s_.GetString()} };
	}

	inline void from_json(nlohmann::json& j_, StringID& s_){
		s_ = StringID::ProcessString(j_);
	}
}

#endif //!GADGET_STRING_ID_H