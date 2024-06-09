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
			Iterator(const Node* node_) : currentNode(node_){}

			constexpr inline const Node* operator*(){ return currentNode; }

			constexpr inline void operator++(){
				if(currentNode != nullptr){
					currentNode = currentNode->next;
				}
			}

			constexpr inline bool operator!=(const Iterator& it_){
				return currentNode != it_.currentNode;
			}

		private:
			const Node* currentNode;
		};

		constexpr List() : size(0), head(nullptr), tail(nullptr){}

		constexpr ~List(){
			while(!IsEmpty()){
				Pop();
			}
		}

		constexpr inline void Add(const T& value_){
			GADGET_BASIC_ASSERT(size < std::numeric_limits<size_t>::max());

			Node* newNode = new Node(value_);

			if(IsEmpty()){
				head = newNode;
				tail = head;
			}else{
				tail->next = new Node(value_);
				tail = tail->next;
			}

			size++;
		}

		constexpr inline void AddFront(const T& value_){
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

		constexpr inline size_t Size() const{ return size; }
		constexpr inline bool IsEmpty() const{ return size == 0; }
		constexpr inline Node* Front(){ return head; }
		constexpr inline Node* Back(){ return tail; }

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

		Iterator begin(){ return Iterator(head); }
		const Iterator begin() const{ return Iterator(head); }
		Iterator end(){ return Iterator(nullptr); }
		const Iterator end() const{ return Iterator(nullptr); }

	private:
		size_t size;
		Node* head;
		Node* tail;
	};
}

#endif //!GADGET_LIST_H