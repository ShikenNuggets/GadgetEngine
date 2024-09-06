#ifndef GADGET_DATA_GRAPH_H
#define GADGET_DATA_GRAPH_H

#include "Data/Array.h"
#include "Data/Queue.h"

namespace Gadget{
	template <class T>
	class Graph{
	public:
		class Node{
		public:
			T value;
			Array<Node*> adjacent;
			bool visited = false;
		};

		Node* AddNode(const T& value_){
			return AddNode(value_, Array<Node*>());
		}

		Node* AddNode(const T& value_, Node* adjacent_){
			GADGET_BASIC_ASSERT(adjacent_ != nullptr);
			Array<Node*> adjacents;
			adjacents.Add(adjacent_);
			return AddNode(value_, adjacents);
		}

		Node* AddNode(const T& value_, const Array<Node*> adjacents_){
			Node* node = new Node();
			node->value = value_;
			node->adjacent = adjacents_;
			node->visited = false;

			for(auto* n : adjacents_){
				GADGET_BASIC_ASSERT(n != nullptr);
				if(n != nullptr){
					n->adjacent.Add(node);
				}
			}

			nodes.Add(node);
			return node;
		}

		Node* GetRoot() const{
			if(nodes.IsEmpty()){
				return nullptr;
			}

			return nodes[0];
		}

		bool SearchDFS(const std::function<bool(Node*)>& visitFunc_, Node* start_ = nullptr){
			//TODO - When do we set visited back to false for all the nodes?
			if(start_ == nullptr){
				if(IsEmpty()){
					return false;
				}

				start_ = GetRoot();
			}

			Node* current = start_;
			
			if(visitFunc_(current)){
				return true;
			}

			current->visited = true;
			for(auto* n : current->adjacent){
				GADGET_BASIC_ASSERT(n != nullptr);
				if(n == nullptr || n->visited){
					continue;
				}

				return SearchDFS(visitFunc_, n);
			}

			return false;
		}

		bool SearchBFS(const std::function<bool(Node*)>& visitFunc_, Node* start_ = nullptr){
			UnvisitAllNodes();
			if(start_ == nullptr){
				if(IsEmpty()){
					return false;
				}

				start_ = GetRoot();
			}

			Queue<Node*> queue;
			Node* current = start_;
			current->visited = true;
			queue.Add(current);

			while(!queue.IsEmpty()){
				Node* r = queue.Remove();
				
				if(visitFunc_(r)){
					return true;
				}

				for(auto* n : r->adjacent){
					GADGET_BASIC_ASSERT(n != nullptr);
					if(n == nullptr || n->visited){
						continue;
					}

					n->visited = true;
					queue.Add(n);
				}
			}

			return false;
		}

		bool HasRoute(Node* start_, Node* end_){
			if(start_ == nullptr || end_ == nullptr){
				return false;
			}

			if(start_ == end_){
				return true;
			}

			return SearchBFS([end_](Node* current_){ return current_ == end_; }, start_);
		}

		constexpr inline int64_t Size() const{ return nodes.Size(); }
		constexpr inline size_t SizeInBytes() const{ return nodes.SizeInBytes(); }
		constexpr inline bool IsEmpty() const{ return nodes.IsEmpty(); }

	private:
		Array<Node*> nodes;

		void UnvisitAllNodes(){
			for(auto* n : nodes){
				n->visited = false;
			}
		}
	};
}

#endif //!GADGET_DATA_GRAPH_H