#ifndef GADGET_DATA_HASH_SET_H
#define GADGET_DATA_HASH_SET_H

#include "Data/Array.h"
#include "Data/List.h"
#include "Data/String.h"
#include "Math/Math.h"

namespace Gadget{
	template <class T>
	class HashSet{
	public:
		class Iterator{
		public:
			constexpr Iterator(const Array<List<T>>& data_, int64_t index_, List<T>::Node* node_) : data(data_), currentIndex(index_), currentNode(node_){}

			constexpr inline const T& operator*() const{ return currentNode->value; }
			constexpr inline T& operator*(){ return currentNode->value; }

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
			const Array<List<T>>& data;
			int64_t currentIndex;
			List<T>::Node* currentNode;
		};

		HashSet(size_t capacity_ = 1024) : data(Math::NextPrime(capacity_)){}

		HashSet(const HashSet<T>& other_) = default;
		HashSet(HashSet<T>&& other_) = default;
		HashSet<T>& operator=(const HashSet<T>& other_) = default;
		HashSet<T>& operator=(HashSet<T>&& other_) = default;
		~HashSet() = default;

		void Add(const T& value_){
			int64_t index = KeyToIndex(value_);
			GADGET_SLOW_ASSERT_MSG(index == KeyToIndex(T(value_)), "HashSet::KeyToIndex providing non-deterministic result!");

			while(index >= data.Size()){
				data.Add(List<T>());
			}

		#ifdef GADGET_DEBUG
			if(data[index].Size() >= warnOnCollisionCount - 1){
				GADGET_LOG_WARNING(SID("DataStructure"), "Performance: HashSet collisions occured. Increasing capacity could improve performance (current capacity is " + std::to_string(data.Capacity()) + ")");
			}
		#endif //GADGET_DEBUG

			for(const auto& v : data[index]){
				if(v->value == value_){
					return; //Value is already in the set
				}
			}

			data[index].Add(value_);
		}

		bool Contains(const T& value_) const{
			int64_t index = KeyToIndex(value_);
			GADGET_SLOW_ASSERT_MSG(index == KeyToIndex(T(value_)), "HashSet::KeyToIndex providing non-deterministic result!");

			if(index >= data.Size()){
				return false;
			}

			for(const auto& v : data[index]){
				if(v->value == value_){
					return true;
				}
			}

			return false;
		}

		void Remove(const T& value_) const{
			int64_t index = KeyToIndex(value_);
			GADGET_SLOW_ASSERT_MSG(index == KeyToIndex(T(value_)), "HashSet::KeyToIndex providing non-deterministic result!");

			if(index >= data.Size()){
				return; //Element is not in the set
			}

			for(const auto& it = data[index].begin(); it != data[index].end(); it++){
				GADGET_BASIC_ASSERT(*it != nullptr);
				if((*it)->value == value_){
					data[index].Remove(it);
					return;
				}
			}
		}

		void Clear(){ data.Clear(); }

		constexpr inline size_t SizeInBytes() const{ return data.SizeInBytes(); }
		constexpr inline bool IsEmpty() const{ return data.IsEmpty(); }

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
		Array<List<T>> data;

		//Log a debug warning if more than N keys have the same hash
		//Too low can trigger many irrelevant warnings, too high will hide legitimate optimization concerns
		//N was chosen somewhat arbitrarily, so adjust this as you see fit
		static constexpr int warnOnCollisionCount = 3;

		int64_t KeyToIndex(const T& key_) const{
			if constexpr(std::is_same_v<T, StringID>){
				return key_.GetID() % data.Capacity();
			}else if constexpr(std::is_enum<T>()){
				GADGET_BASIC_ASSERT(data.Capacity() > static_cast<int64_t>(key_));
				return static_cast<int64_t>(key_);
			}else if constexpr(std::is_same_v<T, String>){
				return Hash::MurmurHash64A(key_.Value(), key_.Length()) % data.Capacity();
			}else{
				return Hash::MurmurHash64A(reinterpret_cast<const char*>(&key_), sizeof(key_)) % data.Capacity();
			}
		}
	};
}

#endif //!GADGET_DATA_HASH_SET_H