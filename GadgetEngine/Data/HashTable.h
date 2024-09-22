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

		class Iterator{
		public:
			constexpr Iterator(const Array<List<KeyValuePair>>& data_, int64_t index_, List<KeyValuePair>::Node* node_) : data(data_), currentIndex(index_), currentNode(node_){}

			constexpr inline const KeyValuePair& operator*() const{ return currentNode->value; }
			constexpr inline KeyValuePair& operator*(){ return currentNode->value; }

			inline Iterator& operator++(){
				if(currentNode != nullptr){
					currentNode = currentNode->next;
				}

				while(currentNode == nullptr){
					currentIndex++;
					if(currentIndex >= data.Size()){
						currentIndex = data.Size();
						break;
					}

					currentNode = data[currentIndex].Front();
				}

				return *this;
			}

			constexpr inline bool operator!=(const Iterator& it_) const{
				return currentNode != it_.currentNode || currentIndex != it_.currentIndex;
			}

			constexpr bool IsValid() const{ return currentNode != nullptr && currentIndex < data.Size(); }

		private:
			const Array<List<KeyValuePair>>& data;
			int64_t currentIndex;
			List<KeyValuePair>::Node* currentNode;
		};

		HashTable(int64_t capacity = 1024) : data(Math::NextPrime(capacity)), numElements(0){}

		HashTable(const HashTable<K, V>& other_) = default;
		HashTable(HashTable<K, V>&& other_) = default;
		HashTable<K, V>& operator=(const HashTable<K, V>& other_) = default;
		HashTable<K, V>& operator=(HashTable<K, V>&& other_) = default;
		~HashTable() = default;

		void Add(const K& key_, const V& value_){
			int64_t index = KeyToIndex(key_);
			GADGET_ASSERT(index == KeyToIndex(key_), "HashTable::KeyToIndex providing non-deterministic result!");
			while(index >= data.Size()){
				data.Add(List<KeyValuePair>());
			}

		#ifdef GADGET_DEBUG
			if(data[index].Size() >= warnOnCollisionCount - 1){
				GADGET_LOG_WARNING(SID("DataStructure"), "Performance: HashTable collision occured. Increasing your hash table size could improve performance (current capacity is " + std::to_string(data.Capacity()) + ")");
			}
		#endif //GADGET_DEBUG

			for(const auto& n : data[index]){
				if(n->value.key == key_){
					GADGET_LOG_WARNING(SID("DATA"), "HashTable::Add called with key that's already present. This can result in memory leaks");
					return; //Overwriting the existing value could result in a memory leak, so we just have to leave
				}
			}

			data[index].Add(KeyValuePair(key_, value_));
			numElements++;
		}

		bool Contains(const K& key_) const{
			int64_t index = KeyToIndex(key_);
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
			int64_t index = KeyToIndex(key_);
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
					numElements--;
					return;
				}
			}
		}

		void Clear(){
			data.Clear();
			numElements = 0;
		}

		constexpr const V& operator[](const K& key_) const{
			GADGET_BASIC_ASSERT(Contains(key_));
			int64_t index = KeyToIndex(key_);
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

		V& operator[](const K& key_){
			int64_t index = KeyToIndex(key_);
			GADGET_ASSERT(index == KeyToIndex(key_), "HashTable::KeyToIndex providing non-deterministic result!");

			if(!Contains(key_)){
				Add(key_, V());
			}

			GADGET_BASIC_ASSERT(Contains(key_));

			for(auto* node : data[index]){
				GADGET_BASIC_ASSERT(node != nullptr);
				if(node->value.key == key_){
					return node->value.value;
				}
			}

			//This shouldn't be possible - there's no safe way to handle this, so just error out
			Debug::ThrowFatalError(SID("DataStructure"), "Tried to get value at unrecognized key!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
		}

		constexpr inline int64_t Size() const{ return numElements; }

		constexpr inline size_t SizeInBytes() const{
			size_t size = sizeof(data);
			for(const auto& l : data){
				size += l.SizeInBytes();
			}

			return size;
		}

		constexpr inline bool IsEmpty() const{ return data.IsEmpty(); }

		constexpr inline Array<K> Keys() const{
			Array<K> keys;

			for(const auto& i : data){
				for(const auto& j : i){
					if(j == nullptr){
						continue;
					}

					keys.Add(j->value.key);
				}
			}

			return keys;
		}

		constexpr inline Iterator begin(){
			if(data.IsEmpty()){
				return Iterator(data, 0, nullptr);
			}

			auto it = Iterator(data, 0, data[0].Front());
			if(!it.IsValid()){
				++it;
			}

			return it;
		}

		constexpr inline Iterator begin() const{
			if(data.IsEmpty()){
				return Iterator(data, 0, nullptr);
			}

			auto it = Iterator(data, 0, data[0].Front());
			if(!it.IsValid()){
				++it;
			}

			return it;
		}

		constexpr inline Iterator end(){ return Iterator(data, data.Size(), nullptr); }
		constexpr inline Iterator end() const{ return Iterator(data, data.Size(), nullptr); }

	private:
		Array<List<KeyValuePair>> data;
		int64_t numElements;

		//Log a debug warning if more than N keys have the same hash
		//Too low can trigger many irrelevant warnings, too high will hide legitimate optimization concerns
		//N was chosen somewhat arbitrarily, so adjust this as you see fit
		static constexpr int warnOnCollisionCount = 3;

		int64_t KeyToIndex(const K& key_) const{
			if constexpr(std::is_same_v<K, StringID>){
				return key_.GetID() % data.Capacity();
			}else if constexpr(std::is_enum<K>()){
				GADGET_BASIC_ASSERT(data.Capacity() > static_cast<int64_t>(key_));
				return static_cast<int64_t>(key_);
			}else{
				return Hash::MurmurHash64A(reinterpret_cast<const char*>(&key_), sizeof(key_)) % data.Capacity();
			}
		}
	};
}

#endif //!GADGET_HASH_TABLE_H