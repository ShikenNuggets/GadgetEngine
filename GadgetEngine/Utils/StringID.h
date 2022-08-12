#ifndef GADGET_STRING_ID_H
#define GADGET_STRING_ID_H

#include <string>
#include <unordered_map>

#include "Debug.h"
#include "Hash.h"

namespace Gadget{
	//TODO - This is an incredibly stupid implementation. Make an actual hash table someday
	class IDTable{
	public:
		static constexpr uint64_t capacity = 1000;

		constexpr IDTable() : size(0), hashes(), strings(){
			for(size_t i = 0; i < capacity; i++){
				hashes[i] = 0;
				strings[i] = "";
			}
		}

		constexpr void Insert(uint64_t hash_, const char* value_){
			_ASSERT(size < capacity); //Bump up the capacity if this ever happens

			hashes[size] = hash_;
			strings[size] = value_;
			size++;
		}

		constexpr size_t FindIndex(uint64_t hash_){
			for(size_t i = 0; i < size; i++){
				if(hashes[i] == hash_){
					return i;
				}
			}

			return std::numeric_limits<size_t>::max();
		}

		constexpr const char* FindValue(uint64_t hash_){
			for(size_t i = 0; i < size; i++){
				if(hashes[i] == hash_){
					return strings[i];
				}
			}

			return "";
		}

		constexpr inline const char* operator[](size_t i_) const{
			_ASSERT(i_ < size);
			return strings[i_];
		}

		constexpr inline const char*& operator[](size_t i_){
			_ASSERT(i_ < size);
			return strings[i_];
		}

	private:
		size_t size;
		uint64_t hashes[capacity];
		const char* strings[capacity];
	};

	struct StringID{
		const uint64_t id;

		explicit constexpr StringID(uint64_t id_) : id(id_){}
		std::string GetString() const;

		static IDTable stringIdTable;

		static constexpr StringID InternString(const std::string& str_){
			StringID id = StringID(Hash::CRC32(str_.data(), str_.length()));

			if(stringIdTable.FindIndex(id.id) >= IDTable::capacity){
				stringIdTable.Insert(id.id, str_.c_str());
			}else{
				Debug::Log("The following string was interned multiple times. Avoid re-internment to improve performance. String was: " + str_, Debug::LogType::Warning);
			}

			return id;
		}

		static constexpr const char* GetStringFromID(StringID id_){
			return stringIdTable.FindValue(id_.id);
		}

		constexpr inline bool operator ==(StringID a_) const{ return id == a_.id; }
		constexpr inline bool operator !=(StringID a_) const{ return id != a_.id; }
		constexpr inline bool operator >(StringID a_) const{ return id > a_.id; }
		constexpr inline bool operator <(StringID a_) const{ return id < a_.id; }
		constexpr inline bool operator >=(StringID a_) const{ return id >= a_.id; }
		constexpr inline bool operator <=(StringID a_) const{ return id <= a_.id; }
	};
}

#endif //!GADGET_STRING_ID_H