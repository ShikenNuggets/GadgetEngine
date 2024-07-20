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

		constexpr ~List(){
			while(!IsEmpty()){
				Pop();
			}
		}

		inline void Add(const T& value_){
			GADGET_BASIC_ASSERT(size < std::numeric_limits<size_t>::max());

			Node* newNode = new Node(value_);

			if(IsEmpty()){
				head = newNode;
				tail = head;
			}else{
				tail->next = newNode;
				tail = tail->next;
			}

			size++;
		}

		inline void AddFront(const T& value_){
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
					prev->next = cur->next;
					delete cur;
					return;
				}
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

			return prev;
		}

		constexpr Iterator begin(){ return Iterator(head); }
		constexpr const Iterator begin() const{ return Iterator(head); }
		constexpr Iterator end(){ return Iterator(nullptr); }
		constexpr const Iterator end() const{ return Iterator(nullptr); }

	private:
		size_t size;
		Node* head;
		Node* tail;
	};
}

#endif //!GADGET_LIST_H