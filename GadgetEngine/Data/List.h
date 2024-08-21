#ifndef GADGET_LIST_H
#define GADGET_LIST_H

#include "Debug.h"

namespace Gadget{
	template <class T>
	class List{
	public:
		struct Node{
			T value;
			Node* next;

			constexpr Node(const T& value_) : value(value_), next(nullptr){}
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

		constexpr List() : size(0), head(nullptr), tail(nullptr){}

		constexpr List(const List<T>& other_) : size(0), head(nullptr), tail(nullptr){
			for(const auto& n : other_){
				Add(n->value);
			}
		}

		constexpr List<T>& operator=(const List<T>& other_){
			if(&other_ == this){
				return *this; //Self-assignment
			}

			while(!IsEmpty()){
				Pop();
			}

			for(const auto& n : other_){
				Add(n->value);
			}

			return *this;
		}

		constexpr ~List(){
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
			GADGET_BASIC_ASSERT(size < std::numeric_limits<size_t>::max());

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
				}

				nodePos_->next = newNode;
				if(nodePos_ == tail){
					tail = newNode;
				}
			}

			size++;
			GADGET_BASIC_ASSERT(tail->next == nullptr);
			GADGET_BASIC_ASSERT(IsValid());
			return newNode;
		}

		inline Node* AddFront(const T& value_){
			GADGET_BASIC_ASSERT(size < std::numeric_limits<size_t>::max());

			Node* newNode = new Node(value_);

			if(IsEmpty()){
				head = newNode;
				tail = head;
			}else{
				newNode->next = head;
				head = newNode;
			}

			size++;

			return newNode;
		}

		//Absorbs all nodes from the other list
		//The other list will be empty after this operation
		inline void Merge(List<T>& other_){
			tail->next = other_->head;
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
			GADGET_BASIC_ASSERT(tail != nullptr);

			tail = SecondLast();
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

			Node* oldHead = head;
			head = head->next;
			delete oldHead;

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

			Node* prev = head;
			Node* cur = head->next;
			while(cur != nullptr){
				if(cur == node_){
					GADGET_BASIC_ASSERT(prev->next == cur);
					prev->next = cur->next;
					delete cur;
					size--;
					return;
				}

				prev = cur;
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
			tail = head;
			Node* current = head;
			Node* prev = nullptr;
			Node* next = nullptr;

			while(current != nullptr){
				next = current->next;
				current->next = prev;
				prev = current;
				current = next;
			}

			head = prev;
		}

		constexpr inline size_t Size() const{ return size; }
		constexpr inline bool IsEmpty() const{ return size == 0; }
		constexpr inline Node* Front() const{ return head; }
		constexpr inline Node* Back() const{ return tail; }

		constexpr inline Node* GetNode(size_t index_) const{
			GADGET_BASIC_ASSERT(index_ < size);

			if(index_ == 0){
				return head;
			}else if(index_ == size - 1){
				return tail;
			}else if(index_ >= size){
				return nullptr;
			}

			Node* current = head;
			for(size_t i = 1; i <= index_; i++){
				current = current->next;
			}

			GADGET_BASIC_ASSERT(current != nullptr);
			return current;
		}

		constexpr inline Node* GetNthLastNode(size_t index_) const{
			GADGET_BASIC_ASSERT(index_ <= size);
			GADGET_BASIC_ASSERT(index_ > 0);
			if(index_ > size || index_ == 0){
				return nullptr;
			}

			if(index_ == 1){
				GADGET_BASIC_ASSERT(tail != nullptr);
				return tail;
			}

			Node* current = nullptr;
			for(size_t i = 0; i < size + 1 - index_; i++){
				if(current == nullptr){
					current = head;
				}else{
					current = current->next;
				}
			}

			GADGET_BASIC_ASSERT(current != nullptr);
			return current;
		}

		constexpr inline Node* SecondLast() const{
			if(IsEmpty()){
				return nullptr;
			}

			Node* prev = nullptr;
			Node* cur = head;
			while(cur != tail && cur != nullptr){
				prev = cur;
				cur = cur->next;
			}

			GADGET_BASIC_ASSERT(prev != nullptr);
			GADGET_BASIC_ASSERT(GetNthLastNode(2) == prev);
			return prev;
		}

		constexpr bool IsValid() const{
			if(Size() == 0 || head == nullptr || tail == nullptr){
				return Size() == 0 && head == nullptr && tail == nullptr; //If any of these are true, they must all be true
			}

			if(Size() == 1 || head == tail || head->next == nullptr){
				return Size() == 1 && head == tail && head->next == nullptr; //If any of these is true, they must all be true
			}

			if(Size() > 1 && head->next == nullptr){
				return false; //head->next must have a value if the list has more than one node
			}

			if(tail->next != nullptr){
				return false; //tail->next must always be null
			}

			Node* current = head;
			size_t count = 0;
			while(current != nullptr){
				count++;
				if(count > Size()){
					return false; //Count got too big, there's either invalid nodes or the list became circular
				}

				if(current->next == nullptr){
					return current == tail && count == Size();
				}

				current = current->next;
			}

			GADGET_ASSERT_UNREACHABLE;
			return false;
		}

		constexpr Iterator begin(){ return Iterator(head); }
		constexpr const Iterator begin() const{ return Iterator(head); }
		constexpr Iterator end(){ return Iterator(nullptr); }
		constexpr const Iterator end() const{ return Iterator(nullptr); }

	private:
		size_t size;
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

#endif //!GADGET_LIST_H