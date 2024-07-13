#ifndef GADGET_HASH_TABLE_H
#define GADGET_HASH_TABLE_H

#include "Debug.h"
#include "Data/Array.h"
#include "Data/List.h"
#include "Math/Math.h"
#include "Utils/Hash.h"

namespace Gadget{
	class StringID;

	template <class K, class V>
	class HashTable{
	public:
		struct KeyValuePair{
			K key;
			V value;

			constexpr KeyValuePair(const K& key_, const V& value_) : key(key_), value(value_){}
		};

		HashTable(size_t capacity = 1024) : data(Math::NextPrime(capacity)){}

		void Add(const K& key_, const V& value_){
			size_t index = KeyToIndex(key_);
			GADGET_ASSERT(index == KeyToIndex(key_), "HashTable::KeyToIndex providing non-deterministic result!");
			while(index >= data.Size()){
				data.Add(List<KeyValuePair>());
			}

		#ifdef GADGET_DEBUG
			if(!data[index].IsEmpty()){
				GADGET_LOG(SID("DataStructure"), "Performance: HashTable collision occured. Increasing your hash table size could improve performance");
			}
		#endif //GADGET_DEBUG

			data[index].Add(KeyValuePair(key_, value_));
		}

		bool Contains(const K& key_) const{
			size_t index = KeyToIndex(key_);
			GADGET_ASSERT(index == KeyToIndex(key_), "HashTable::KeyToIndex providing non-deterministic result!");
			if(index >= data.Size()){
				return false;
			}

			for(const auto& node : data[index]){
				GADGET_BASIC_ASSERT(node != nullptr);
				if(node->value.key == key_){
					return true;
				}
			}

			return false;

		}

		void RemoveAt(const K& key_){
			GADGET_BASIC_ASSERT(Contains(key_));
			size_t index = KeyToIndex(key_);
			GADGET_ASSERT(index == KeyToIndex(key_), "HashTable::KeyToIndex providing non-deterministic result!");
			if(index >= data.Size()){
				GADGET_LOG_WARNING(SID("DataStructure"), "Tried to remove value at unknown key!");
				return;
			}

			GADGET_BASIC_ASSERT(!data[index].IsEmpty());
			if(data[index].IsEmpty()){
				GADGET_LOG_WARNING(SID("DataStructure"), "Tried to remove value at unknown key!");
				return;
			}

			for(const auto& node : data[index]){
				GADGET_BASIC_ASSERT(node != nullptr);
				if(node->value.key == key_){
					data[index].Remove(node);
					return;
				}
			}
		}

		constexpr const V& operator[](const K& key_) const{
			GADGET_BASIC_ASSERT(Contains(key_));
			size_t index = KeyToIndex(key_);
			GADGET_ASSERT(index == KeyToIndex(key_), "HashTable::KeyToIndex providing non-deterministic result!");
			if(index >= data.Size()){
				//There's no safe way to handle this request, so just error out
				Debug::ThrowFatalError(SID("DataStructure"), "Tried to get value at unrecognized key!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
			}

			for(const auto* node : data[index]){
				GADGET_BASIC_ASSERT(node != nullptr);
				if(node->value.key == key_){
					return node->value.value;
				}
			}

			//There's no safe way to handle this request, so just error out
			Debug::ThrowFatalError(SID("DataStructure"), "Tried to get value at unrecognized key!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
		}

	private:
		Array<List<KeyValuePair>> data;

		size_t KeyToIndex(const K& key_) const{
			if constexpr(std::is_same_v<K, StringID>){
				return key_.GetID() % data.Capacity();
			}else{
				return Hash::MurmurHash64A(reinterpret_cast<const char*>(&key_), sizeof(key_)) % data.Capacity();
			}
		}
	};
}

#endif //!GADGET_HASH_TABLE_H