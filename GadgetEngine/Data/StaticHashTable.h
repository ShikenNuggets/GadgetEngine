#ifndef GADGET_DATA_STATIC_HASH_TABLE_H
#define GADGET_DATA_STATIC_HASH_TABLE_H

#include <type_traits>

#include "Debug.h"
#include "Data/StaticArray.h"

namespace Gadget{
	class StringID;

	static constexpr inline int64_t gDefaultStaticHashTableSize = 1031;

	template <class K, class V, int64_t Size = gDefaultStaticHashTableSize>
	class StaticHashTable{
	public:
		struct KeyValuePair{
			K key;
			V value;
			bool isInUse;

			constexpr KeyValuePair() : key(), value(), isInUse(false){}
			constexpr KeyValuePair(const K& key_, const V& value_) : key(key_), value(value_), isInUse(true){}
		};

		class Iterator{
		public:
			constexpr Iterator(StaticArray<KeyValuePair, Size>& data_, int64_t index_) : data(data_), index(index_){}

			constexpr inline const V& operator*() const{ return data[index].value; }
			constexpr inline V& operator*(){ return data[index].value; }

			constexpr inline Iterator& operator++(){
				do{
					index++;
				}while(index < Size && !data[index].isInUse);
				return *this;
			}

			constexpr inline bool operator!=(const Iterator& it_) const{ return index != it_.index; }

			constexpr bool IsValid() const{ return data[index].isInUse && index < Size; }

		private:
			StaticArray<KeyValuePair, Size>& data;
			int64_t index;
		};

		StaticHashTable() : data(), elementsInUse(0){}

		void Add(const K& key_, const V& value_){
			int64_t index = KeyToIndex(key_);
			GADGET_SLOW_ASSERT_MSG(index == KeyToIndex(key_), "StaticHashTable::KeyToIndex providing non-deterministic result!");
			GADGET_BASIC_ASSERT(index < Size);
			GADGET_BASIC_ASSERT(data[index].isInUse == false);
			if(data[index].isInUse){
				GADGET_LOG_WARNING(SID("DataStructure"), "HashTable collision occured, value has not been added!");
			}else{
				data[index] = KeyValuePair(key_, value_);
				elementsInUse++;
			}
		}

		bool Contains(const K& key_) const{
			int64_t index = KeyToIndex(key_);
			GADGET_SLOW_ASSERT_MSG(index == KeyToIndex(key_), "StaticHashTable::KeyToIndex providing non-deterministic result!");
			GADGET_BASIC_ASSERT(index < Size);

			return data[index].isInUse && data[index].key == key_;
		}

		void RemoveAt(const K& key_){
			int64_t index = KeyToIndex(key_);
			GADGET_SLOW_ASSERT_MSG(index == KeyToIndex(key_), "StaticHashTable::KeyToIndex providing non-deterministic result!");
			GADGET_BASIC_ASSERT(index < Size);

			GADGET_BASIC_ASSERT(data[index].key == key_);
			data[index].isInUse = false;
			elementsInUse--;
		}

		void Clear(){
			for(auto& t : data){
				t.isInUse = false;
			}

			elementsInUse = 0;
		}

		const V& operator[](const K& key_) const{
			GADGET_BASIC_ASSERT(Contains(key_));
			int64_t index = KeyToIndex(key_);
			GADGET_SLOW_ASSERT_MSG(index == KeyToIndex(key_), "StaticHashTable::KeyToIndex providing non-deterministic result!");
			GADGET_BASIC_ASSERT(index < Size);

			GADGET_BASIC_ASSERT(data[index].key == key_);
			if(!data[index].isInUse || data[index].key != key_){
				//There's no safe way to handle this request, so just error out
				Debug::ThrowFatalError(SID("DataStructure"), "Tried to get value at unrecognized key!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
			}

			return data[index].value;
		}

		V& operator[](const K& key_){
			int64_t index = KeyToIndex(key_);
			GADGET_SLOW_ASSERT_MSG(index == KeyToIndex(key_), "StaticHashTable::KeyToIndex providing non-deterministic result!");
			GADGET_BASIC_ASSERT(index < Size);

			if(!Contains(key_)){
				Add(key_, V());
			}

			GADGET_BASIC_ASSERT(Contains(key_));
			GADGET_BASIC_ASSERT(data[index].key == key_);
			if(!data[index].isInUse || data[index].key != key_){
				//There's no safe way to handle this request, so just error out
				Debug::ThrowFatalError(SID("DataStructure"), "Tried to get value at unrecognized key!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
			}

			return data[index].value;
		}

		constexpr inline bool IsEmpty() const{ return elementsInUse == 0; }
		constexpr inline size_t SizeInBytes() const{ return sizeof(*this); }

		constexpr inline Iterator begin(){
			if(IsEmpty()){
				return Iterator(data, 0);
			}

			auto it = Iterator(data, 0);
			if(!it.IsValid()){
				++it;
			}

			return it;
		}

		constexpr inline Iterator begin() const{
			if(IsEmpty()){
				return Iterator(data, 0);
			}

			auto it = Iterator(data, 0);
			if(!it.IsValid()){
				++it;
			}

			return it;
		}

		constexpr inline Iterator end(){ return Iterator(data, Size); }
		constexpr inline Iterator end() const{ return Iterator(data, Size); }

	private:
		StaticArray<KeyValuePair, Size> data;
		int64_t elementsInUse;

		int64_t KeyToIndex(const K& key_) const{
			if constexpr(std::is_same_v<K, StringID>){
				return key_.GetID() % Size;
			}else if constexpr(std::is_enum<K>()){
				GADGET_BASIC_ASSERT(Size > static_cast<int64_t>(key_));
				return static_cast<int64_t>(key_);
			}else{
				return Hash::MurmurHash64A(reinterpret_cast<const char*>(&key_), sizeof(key_)) % Size;
			}
		}
	};
}

#endif //!GADGET_DATA_STATIC_HASH_TABLE_H