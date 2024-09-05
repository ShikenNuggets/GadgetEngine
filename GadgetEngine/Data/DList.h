#ifndef GADGET_D_LIST_H
#define GADGET_D_LIST_H

#include "Debug.h"

namespace Gadget{
	//Doubly-linked list
	template <class T>
	class DList{
	public:
		struct Node{
			T value;
			Node* next;
			Node* prev;

			constexpr Node(const T& value_) : value(value_), next(nullptr), prev(nullptr){}
		};

		class Iterator{
		public:
			constexpr Iterator(Node* node_) : currentNode(node_){}

			constexpr inline const Node* operator*() const{ return currentNode; }
			constexpr inline Node* operator*(){ return currentNode; }

			constexpr inline Iterator& operator++(){
				if(currentNode != nullptr){
					currentNode = currentNode->next;
				}

				return *this;
			}

			constexpr inline bool operator!=(const Iterator& it_) const{
				return currentNode != it_.currentNode;
			}

		private:
			Node* currentNode;
		};

		constexpr DList() : size(0), head(nullptr), tail(nullptr){}

		constexpr DList(const DList<T>& other_) : size(0), head(nullptr), tail(nullptr){
			for(const auto& n : other_){
				Add(n->value);
			}
		}

		constexpr DList(DList<T>&& other_) noexcept : size(other_.size), head(other_.head), tail(other_.tail){
			other_.size = 0;
			other_.head = nullptr;
			other_.tail = nullptr;

			GADGET_BASIC_ASSERT(IsValid());
			GADGET_BASIC_ASSERT(other_.IsEmpty());
			GADGET_BASIC_ASSERT(other_.IsValid());
		}

		constexpr DList<T>& operator=(const DList<T>& other_){
			if(&other_ == this){
				return *this; //Self-assignment
			}

			Clear();

			for(const auto& n : other_){
				Add(n->value);
			}

			return *this;
		}

		constexpr DList<T>& operator=(DList<T>&& other_) noexcept{
			if(&other_ == this){
				return *this; //Self-assignment
			}

			Clear();

			size = other_.size;
			head = other_.head;
			tail = other_.tail;

			other_.size = 0;
			other_.head = nullptr;
			other_.tail = nullptr;

			GADGET_BASIC_ASSERT(IsValid());
			GADGET_BASIC_ASSERT(other_.IsEmpty());
			GADGET_BASIC_ASSERT(other_.IsValid());

			return *this;
		}

		constexpr ~DList(){
			while(!IsEmpty()){
				Pop();
			}
		}

		inline Node* Add(const T& value_){
			return Add(value_, tail);
		}

		inline Node* Add(const T& value_, Node* nodePos_){
			GADGET_BASIC_ASSERT(IsEmpty() || nodePos_ != nullptr);
			GADGET_BASIC_ASSERT(nodePos_ == nullptr || ContainsNode(nodePos_));
			GADGET_BASIC_ASSERT(size < std::numeric_limits<int64_t>::max());

			Node* newNode = new Node(value_);

			if(IsEmpty()){
				head = newNode;
				tail = head;
			}else{
				if(nodePos_ == nullptr){
					GADGET_LOG_ERROR(SID("DATA"), "Tried to add at invalid node position!");
					delete newNode;
					return nullptr;
				}

				if(nodePos_->next != nullptr){
					newNode->next = nodePos_->next;
					newNode->next->prev = newNode;
				}

				nodePos_->next = newNode;
				newNode->prev = nodePos_;
				if(nodePos_ == tail){
					tail = newNode;
				}
			}

			size++;
			GADGET_BASIC_ASSERT(head->prev == nullptr);
			GADGET_BASIC_ASSERT(tail->next == nullptr);
			GADGET_BASIC_ASSERT(IsValid());
			return newNode;
		}

		inline Node* AddFront(const T& value_){
			GADGET_BASIC_ASSERT(size < std::numeric_limits<int64_t>::max());

			Node* newNode = new Node(value_);

			if(IsEmpty()){
				head = newNode;
				tail = head;
			}else{
				newNode->next = head;
				head->prev = newNode;
				head = newNode;
			}

			size++;
			return newNode;
		}

		//Absorbs all nodes from the other list
		//The other list will be empty after this operation
		inline void Merge(DList<T>& other_){
			tail->next = other_->head;
			other_->head->prev = tail;
			tail = other_->tail;
			size += other_.size;

			//Empty the other list so there's no ambiguity on ownership
			other_.head = nullptr;
			other_.tail = nullptr;
			other_.size = 0;

			GADGET_BASIC_ASSERT(IsValid());
			GADGET_BASIC_ASSERT(other_.IsEmpty());
			GADGET_BASIC_ASSERT(other_.IsValid());
		}

		constexpr inline void Pop(){
			if(IsEmpty()){
				return;
			}else if(size == 1){
				delete head;
				head = nullptr;
				tail = nullptr;
				size--;
				return;
			}

			GADGET_BASIC_ASSERT(size > 0);
			GADGET_BASIC_ASSERT(tail != nullptr && tail->prev != nullptr);
			if(size == 0 || tail == nullptr || tail->prev == nullptr){
				Debug::ThrowFatalError(SID("DATA"), "An error occurred while trying to pop from a doubly-linked lists!", ErrorCode::Unknown, __FILE__, __LINE__);
			}

			tail = tail->prev;
			delete tail->next;
			tail->next = nullptr;

			size--;
		}

		constexpr inline void PopFront(){
			if(IsEmpty()){
				return;
			}else if(size == 1){
				delete head;
				head = nullptr;
				tail = nullptr;
				size--;
				return;
			}

			GADGET_BASIC_ASSERT(size > 0);
			GADGET_BASIC_ASSERT(head != nullptr);

			head = head->next;
			delete head->prev;
			head->prev = nullptr;

			size--;
		}

		//Removes the specified node from the list
		//If successful, the Node* passed in will not be valid afterwards
		constexpr inline void Remove(const Node* node_){
			GADGET_BASIC_ASSERT(node_ != nullptr);

			if(IsEmpty()){
				GADGET_LOG_WARNING(SID("DataStructures"), "Tried to remove a node from an empty list");
				return;
			}

			if(node_ == head){
				PopFront();
				return;
			}else if(node_ == tail){
				Pop();
				return;
			}

			Node* cur = head->next;
			while(cur != nullptr){
				if(cur == node_){
					if(cur->prev != nullptr){
						GADGET_BASIC_ASSERT(cur->prev->next == cur);
						cur->prev->next = cur->next;
					}

					if(cur->next != nullptr){
						GADGET_BASIC_ASSERT(cur->next->prev == cur);
						cur->next->prev = cur->prev;
					}

					delete cur;
					size--;
					return;
				}

				cur = cur->next;
			}

			GADGET_LOG_WARNING(SID("DataStructures"), "Tried to remove a node from a list, but could not find the node - There's either a bug in your code, a bug in the List code, or List nodes were manually altered by other code (do not do that)");
		}

		constexpr inline void Clear(){
			while(!IsEmpty()){
				Pop();
			}
		}

		constexpr inline void Reverse(){
			Node* current = head;
			while(current != nullptr){
				Node* oldNext = current->next;
				current->next = current->prev;
				current->prev = oldNext;

				current = oldNext;
			}

			Node* oldHead = head;
			head = tail;
			tail = oldHead;
		}

		constexpr inline int64_t Size() const{ return size; }
		constexpr inline size_t SizeInBytes() const{ return sizeof(DList<T>) + (sizeof(Node) * size); }
		constexpr inline bool IsEmpty() const{ return size == 0; }
		constexpr inline Node* Front() const{ return head; }
		constexpr inline Node* Back() const{ return tail; }

		constexpr inline Node* GetNode(int64_t index_) const{
			GADGET_BASIC_ASSERT(index_ < size);

			if(index_ == 0){
				return head;
			}else if(index_ == size - 1){
				return tail;
			}else if(index_ >= size){
				return nullptr;
			}

			Node* current = head;
			for(int64_t i = 1; i <= index_; i++){
				current = current->next;
			}

			GADGET_BASIC_ASSERT(current != nullptr);
			return current;
		}

		constexpr inline Node* GetNthLastNode(int64_t index_) const{
			GADGET_BASIC_ASSERT(index_ <= size);
			GADGET_BASIC_ASSERT(index_ > 0);
			if(index_ > size || index_ == 0){
				return nullptr;
			}

			if(index_ == 1){
				GADGET_BASIC_ASSERT(tail != nullptr);
				return tail;
			}

			Node* current = tail;
			for(int64_t i = 1; i < index_; i++){
				current = current->prev;
			}

			GADGET_BASIC_ASSERT(current != nullptr);
			return current;
		}

		constexpr inline Node* SecondLast() const{
			if(IsEmpty()){
				return nullptr;
			}

			GADGET_BASIC_ASSERT(GetNthLastNode(2) == tail->prev);
			return tail->prev;
		}

		constexpr bool IsValid() const{
			if(Size() == 0 || head == nullptr || tail == nullptr){
				return Size() == 0 && head == nullptr && tail == nullptr; //If any of these are true, they must all be true
			}

			if(Size() == 1 || head == tail || head->next == nullptr || tail->prev == nullptr){
				return Size() == 1 && head == tail && head->next == nullptr && tail->prev == nullptr; //If any of these is true, they must all be true
			}

			if(Size() > 1 && head->next == nullptr){
				return false; //head->next must have a value if the list has more than one node
			}else if(Size() > 1 && tail->prev == nullptr){
				return false; //tail->prev must have a value if the list has more than one node
			}

			if(tail->next != nullptr){
				return false; //tail->next must always be null
			}

			Node* current = head;
			int64_t count = 0;
			while(current != nullptr){
				count++;
				if(count > Size()){
					return false; //Count got too big, there's either invalid nodes or the list became circular
				}

				if(current->prev == nullptr && current != head){
					return false;
				}

				if(current->next == nullptr && current != tail){
					return false;
				}

				current = current->next;
			}

			if(count != Size()){
				return false;
			}

			current = tail;
			count = 0;
			while(current != nullptr){
				count++;
				if(count > Size()){
					return false; //Count got too big, there's either invalid nodes or the list became circular
				}

				if(current->prev == nullptr && current != head){
					return false;
				}

				if(current->next == nullptr && current != tail){
					return false;
				}

				current = current->prev;
			}

			return count == Size();
		}

		constexpr Iterator begin(){ return Iterator(head); }
		constexpr const Iterator begin() const{ return Iterator(head); }
		constexpr Iterator end(){ return Iterator(nullptr); }
		constexpr const Iterator end() const{ return Iterator(nullptr); }

	private:
		int64_t size;
		Node* head;
		Node* tail;

		constexpr bool ContainsNode(Node* node_) const{
			GADGET_BASIC_ASSERT(node_ != nullptr);

			for(const auto* n : *this){
				if(n == node_){
					return true;
				}
			}

			return false;
		}
	};
}

#endif //!GADGET_D_LIST_H