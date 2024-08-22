#include <Gadget.h>
#include <Data/Array.h>
#include <Data/DList.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//------------ Add Two Numbers (Leetcode 2) ------------------//
//------------------------------------------------------------//
static inline DList<int> AddTwoNumbers(const DList<int>& l1_, const DList<int>& l2_){
	GADGET_BASIC_ASSERT(l1_.IsValid());
	GADGET_BASIC_ASSERT(l2_.IsValid());

	Array<int> l1Digits;
	Array<int> l2Digits;

	for(const auto& d : l1_){
		GADGET_BASIC_ASSERT(d != nullptr);
		l1Digits.Add(d->value);
	}

	for(const auto& d : l2_){
		l2Digits.Add(d->value);
	}

	size_t totalDigits = std::max(l1Digits.Size(), l2Digits.Size());

	Array<int> finalDigits = Array<int>(totalDigits);
	int extra = 0;
	for(size_t i = 0; i < totalDigits; i++){
		int newDigit = extra;
		extra = 0;

		if(i < l1Digits.Size()){
			newDigit += l1Digits[i];
		}

		if(i < l2Digits.Size()){
			newDigit += l2Digits[i];
		}

		while(newDigit >= 10){
			extra++;
			newDigit -= 10;
		}

		finalDigits.Add(newDigit);
	}

	while(extra != 0){
		int newDigit = extra;
		extra = 0;

		while(newDigit >= 10){
			extra++;
			newDigit -= 10;
		}

		finalDigits.Add(newDigit);
	}

	DList<int> finalList;
	GADGET_BASIC_ASSERT(finalList.IsValid());
	for(size_t i = 0; i < finalDigits.Size(); i++){
		finalList.Add(finalDigits[i]);
		GADGET_BASIC_ASSERT(finalList.IsValid());
	}

	return finalList;
}

TEST_CASE("DList - Add Two Numbers", "[dlist_add_two_numbers]"){
	DList<int> list1;
	list1.Add(2);
	list1.Add(4);
	list1.Add(3);
	REQUIRE(list1.IsValid());

	DList<int> list2;
	list2.Add(5);
	list2.Add(6);
	list2.Add(4);
	REQUIRE(list2.IsValid());

	DList<int> result1 = AddTwoNumbers(list1, list2);
	REQUIRE(result1.IsValid());
	REQUIRE(result1.IsEmpty() == false);
	REQUIRE(result1.Front() != nullptr);
	REQUIRE(result1.Front()->next != nullptr);
	REQUIRE(result1.Front()->next->next != nullptr);
	REQUIRE(result1.Front()->value == 7);
	REQUIRE(result1.Front()->next->value == 0);
	REQUIRE(result1.Front()->next->next->value == 8);

	list1.Clear();
	list1.Add(0);
	REQUIRE(list1.IsValid());

	list2.Clear();
	list2.Add(0);
	REQUIRE(list2.IsValid());

	DList<int> result2 = AddTwoNumbers(list1, list2);
	REQUIRE(result2.IsValid());
	REQUIRE(result2.IsEmpty() == false);
	REQUIRE(result2.Front() != nullptr);
	REQUIRE(result2.Front()->value == 0);

	list1.Clear();
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	REQUIRE(list1.IsValid());

	list2.Clear();
	list2.Add(9);
	list2.Add(9);
	list2.Add(9);
	list2.Add(9);
	REQUIRE(list2.IsValid());

	DList<int> result3 = AddTwoNumbers(list1, list2);
	REQUIRE(result3.IsValid());
	REQUIRE(result3.IsEmpty() == false);
	REQUIRE(result3.Front() != nullptr);
	REQUIRE(result3.Front()->next != nullptr);
	REQUIRE(result3.Front()->next->next != nullptr);
	REQUIRE(result3.Front()->next->next->next != nullptr);
	REQUIRE(result3.Front()->next->next->next != nullptr);
	REQUIRE(result3.Front()->next->next->next->next != nullptr);
	REQUIRE(result3.Front()->next->next->next->next->next != nullptr);
	REQUIRE(result3.Front()->next->next->next->next->next->next != nullptr);

	REQUIRE(result3.Front()->value == 8);
	REQUIRE(result3.Front()->next->value == 9);
	REQUIRE(result3.Front()->next->next->value == 9);
	REQUIRE(result3.Front()->next->next->next->value == 9);
	REQUIRE(result3.Front()->next->next->next->next->value == 0);
	REQUIRE(result3.Front()->next->next->next->next->next->value == 0);
	REQUIRE(result3.Front()->next->next->next->next->next->next->value == 0);
	REQUIRE(result3.Front()->next->next->next->next->next->next->next->value == 1);
}

//------------------------------------------------------------//
//--------------- Remove Dupes (CTCI 2.1) --------------------//
//------------------------------------------------------------//
//O(n^2)
static inline void RemoveDupes1(DList<int>& list_){
	DList<int>::Node* current = list_.Front();

	while(current != nullptr){
		DList<int>::Node* next = current->next;

		while(next != nullptr){
			if(current->value == next->value){
				DList<int>::Node* oldNext = next;
				next = next->next;
				list_.Remove(oldNext);
				continue;
			}

			next = next->next;
		}

		current = current->next;
	}
}

//O(n) with significant time complexity tradeoff
static inline void RemoveDupes2(DList<int>& list_){
	HashTable<int, int> uniques;

	DList<int>::Node* current = list_.Front();
	while(current != nullptr){
		if(uniques.Contains(current->value)){
			DList<int>::Node* old = current;
			current = current->next;
			list_.Remove(old);
			continue;
		}

		uniques.Add(current->value, current->value);
		current = current->next;
	}
}

TEST_CASE("DList Remove Dupes", "[dlist_remove_dupes]"){
	DList<int> list1;
	list1.Add(4);
	list1.Add(5);
	list1.Add(6);
	list1.Add(7);
	list1.Add(8);

	DList<int> list1_2 = list1;

	REQUIRE(list1.Size() == 5);
	REQUIRE(list1.IsValid());
	REQUIRE(list1_2.Size() == 5);
	REQUIRE(list1_2.IsValid());

	RemoveDupes1(list1);
	REQUIRE(list1.IsValid());
	REQUIRE(list1.Size() == 5);
	RemoveDupes1(list1);
	REQUIRE(list1.IsValid());
	REQUIRE(list1.Size() == 5);

	RemoveDupes2(list1_2);
	REQUIRE(list1_2.IsValid());
	REQUIRE(list1_2.Size() == 5);
	RemoveDupes2(list1_2);
	REQUIRE(list1_2.IsValid());
	REQUIRE(list1_2.Size() == 5);

	DList<int>::Node* current1 = list1.Front();
	DList<int>::Node* current2 = list1_2.Front();
	size_t count = 0;
	while(current1 != nullptr && current2 != nullptr){
		REQUIRE(current1->value == current2->value);
		current1 = current1->next;
		current2 = current2->next;
		count++;
	}

	REQUIRE(count == 5);

	//---------------------------------//

	DList<int> list2;
	list2.Add(4);
	list2.Add(4);
	list2.Add(5);
	list2.Add(4);
	list2.Add(5);

	DList<int> list2_2 = list2;

	REQUIRE(list2.Size() == 5);
	REQUIRE(list2.IsValid());
	REQUIRE(list2_2.Size() == 5);
	REQUIRE(list2_2.IsValid());

	RemoveDupes1(list2);
	REQUIRE(list2.IsValid());
	REQUIRE(list2.Size() == 2);
	RemoveDupes1(list2);
	REQUIRE(list2.IsValid());
	REQUIRE(list2.Size() == 2);

	RemoveDupes2(list2_2);
	REQUIRE(list2_2.IsValid());
	REQUIRE(list2_2.Size() == 2);
	RemoveDupes2(list2_2);
	REQUIRE(list2_2.IsValid());
	REQUIRE(list2_2.Size() == 2);

	current1 = list2.Front();
	current2 = list2_2.Front();
	count = 0;
	while(current1 != nullptr && current2 != nullptr){
		REQUIRE(current1->value == current2->value);
		current1 = current1->next;
		current2 = current2->next;
		count++;
	}

	REQUIRE(count == 2);
}

//------------------------------------------------------------//
//-------------- Kth to Last Node (CTCI 2.2) -----------------//
//------------------------------------------------------------//

static inline DList<int>::Node* GetNthLastNode_UnknownSize(DList<int>::Node* const head, size_t nthLast){
	GADGET_BASIC_ASSERT(head != nullptr);

	DList<int>::Node* current = head;
	DList<int>::Node* skipNode = head;

	size_t currentIndex = 0;
	size_t size = 0;
	while(skipNode != nullptr){
		GADGET_BASIC_ASSERT(current != nullptr);
		current = current->next;
		currentIndex++;

		if(skipNode->next == nullptr){
			size++;
			break;
		}

		skipNode = skipNode->next->next;
		size += 2;
	}

	GADGET_BASIC_ASSERT(nthLast <= size);
	size_t targetIndex = size - nthLast;
	if(currentIndex > targetIndex){
		//We went too far. Start over
		current = head;
		currentIndex = 0;
	}

	for(size_t i = currentIndex; i < targetIndex; i++){
		GADGET_BASIC_ASSERT(current != nullptr);
		if(current == nullptr){
			break;
		}

		current = current->next;
	}

	return current;
}

TEST_CASE("DList Kth to Last Node", "[dlist_k_to_last]"){
	DList<int> list;
	list.Add(4);
	list.Add(5);
	list.Add(6);
	list.Add(7);
	list.Add(8);

	REQUIRE(list.GetNthLastNode(1) != nullptr);
	REQUIRE(list.GetNthLastNode(1)->value == 8);
	REQUIRE(list.GetNthLastNode(1) == list.Back());
	REQUIRE(list.GetNthLastNode(1)->next == nullptr);
	REQUIRE(list.GetNthLastNode(1) == GetNthLastNode_UnknownSize(list.Front(), 1));

	REQUIRE(list.GetNthLastNode(2) != nullptr);
	REQUIRE(list.GetNthLastNode(2)->value == 7);
	REQUIRE(list.GetNthLastNode(2)->next == list.Back());
	REQUIRE(list.GetNthLastNode(2)->next->next == nullptr);
	REQUIRE(list.GetNthLastNode(2) == GetNthLastNode_UnknownSize(list.Front(), 2));

	REQUIRE(list.GetNthLastNode(3) != nullptr);
	REQUIRE(list.GetNthLastNode(3)->value == 6);
	REQUIRE(list.GetNthLastNode(3)->next->next == list.Back());
	REQUIRE(list.GetNthLastNode(3)->next->next->next == nullptr);
	REQUIRE(list.GetNthLastNode(3) == GetNthLastNode_UnknownSize(list.Front(), 3));

	REQUIRE(list.GetNthLastNode(4) != nullptr);
	REQUIRE(list.GetNthLastNode(4)->value == 5);
	REQUIRE(list.GetNthLastNode(4)->next->next->next == list.Back());
	REQUIRE(list.GetNthLastNode(4)->next->next->next->next == nullptr);
	REQUIRE(list.GetNthLastNode(4) == GetNthLastNode_UnknownSize(list.Front(), 4));

	REQUIRE(list.GetNthLastNode(5) != nullptr);
	REQUIRE(list.GetNthLastNode(5)->value == 4);
	REQUIRE(list.GetNthLastNode(5)->next->next->next->next == list.Back());
	REQUIRE(list.GetNthLastNode(5)->next->next->next->next->next == nullptr);
	REQUIRE(list.GetNthLastNode(5) == GetNthLastNode_UnknownSize(list.Front(), 5));
}

//------------------------------------------------------------//
//----------------- Partition (CTCI 2.4) ---------------------//
//------------------------------------------------------------//

void PartitionListAtValue(DList<int>& list, int pValue){
	GADGET_BASIC_ASSERT(!list.IsEmpty());
	if(list.IsEmpty()){
		return;
	}

	DList<int> pList;
	GADGET_BASIC_ASSERT(pList.IsValid());
	DList<int>::Node* lowPartition = nullptr;
	DList<int>::Node* highPartition = nullptr;

	pList.Add(list.Front()->value);
	GADGET_BASIC_ASSERT(pList.IsValid());
	if(list.Front()->value < pValue){
		lowPartition = pList.Front();
	}else{
		highPartition = pList.Front();
	}

	bool firstIter = true;
	for(const auto* n : list){
		if(firstIter){
			firstIter = false;
			continue;
		}

		if(lowPartition == nullptr && n->value < pValue){
			pList.AddFront(n->value);
			GADGET_BASIC_ASSERT(pList.IsValid());
			lowPartition = pList.Front();
			continue;
		} else if(highPartition == nullptr && n->value >= pValue){
			pList.Add(n->value);
			GADGET_BASIC_ASSERT(pList.IsValid());
			highPartition = pList.Back();
			continue;
		}

		if(n->value < pValue){
			pList.Add(n->value, lowPartition);
			GADGET_BASIC_ASSERT(pList.IsValid());
		}else{
			DList<int>::Node* newNode = pList.Add(n->value, highPartition);
			GADGET_BASIC_ASSERT(pList.IsValid());
			if(n->value == pValue){
				highPartition = newNode; //This technically isn't necessary to meet the requirements of this function, but keeping the pValues together is ideal
			}
		}
	}

	list = pList;
	GADGET_BASIC_ASSERT(list.IsValid());
}

TEST_CASE("DList Partition", "[dlist_partition]"){
	DList<int> inputList;
	inputList.Add(3);
	inputList.Add(5);
	inputList.Add(8);
	inputList.Add(5);
	inputList.Add(10);
	inputList.Add(2);
	inputList.Add(1);
	REQUIRE(inputList.IsValid());

	PartitionListAtValue(inputList, 5);
	REQUIRE(inputList.IsValid());

	REQUIRE(inputList.Front() != nullptr);
	REQUIRE(inputList.Front()->next != nullptr);
	REQUIRE(inputList.Front()->next->next != nullptr);
	REQUIRE(inputList.Front()->next->next->next != nullptr);
	REQUIRE(inputList.Front()->next->next->next->next != nullptr);
	REQUIRE(inputList.Front()->next->next->next->next->next != nullptr);
	REQUIRE(inputList.Front()->next->next->next->next->next->next != nullptr);

	REQUIRE(inputList.Front()->value == 3);
	REQUIRE(inputList.Front()->next->value == 1);
	REQUIRE(inputList.Front()->next->next->value == 2);
	REQUIRE(inputList.Front()->next->next->next->value == 5);
	REQUIRE(inputList.Front()->next->next->next->next->value == 5);
	REQUIRE(inputList.Front()->next->next->next->next->next->value == 10);
	REQUIRE(inputList.Front()->next->next->next->next->next->next->value == 8);
}

//------------------------------------------------------------//
//----------------- Sum Lists (CTCI 2.5) ---------------------//
//------------------------------------------------------------//

static inline int SumNodes(DList<int>::Node* aCur, DList<int>::Node* bCur, DList<int>& result){
	int sum = 0;
	if(aCur == nullptr && bCur == nullptr){
		return 0;
	} else if(aCur && !bCur){
		sum += SumNodes(aCur->next, nullptr, result);
		sum += aCur->value;
	} else if(bCur && !aCur){
		sum += SumNodes(nullptr, bCur->next, result);
		sum += bCur->value;
	} else{
		sum += SumNodes(aCur->next, bCur->next, result);
		sum += aCur->value + bCur->value;
	}

	int remainder = 0;
	while(sum >= 10){
		remainder++;
		sum -= 10;
	}

	result.AddFront(sum);
	return remainder;
}

static inline DList<int> SumLists(const DList<int>& a, const DList<int>& b){
	DList<int> resultTest;
	SumNodes(a.Front(), b.Front(), resultTest);
	return resultTest;
}

static inline DList<int> SumListsReverse(const DList<int>& a, const DList<int>& b){
	DList<int>::Node* aCurrent = a.Front();
	DList<int>::Node* bCurrent = b.Front();

	DList<int> result;
	int remainder = 0;
	while(aCurrent != nullptr || bCurrent != nullptr){
		int sum = remainder;
		remainder = 0;

		if(aCurrent != nullptr){
			sum += aCurrent->value;
			aCurrent = aCurrent->next;
		}

		if(bCurrent != nullptr){
			sum += bCurrent->value;
			bCurrent = bCurrent->next;
		}

		while(sum > 10){
			remainder++;
			sum -= 10;
		}

		result.Add(sum);
	}

	if(remainder > 0){
		result.Add(remainder);
	}

	return result;
}

TEST_CASE("DList Sum", "[dlist_sum]"){
	DList<int> listA;
	listA.Add(7);
	listA.Add(1);
	listA.Add(6);

	DList<int> listB;
	listB.Add(5);
	listB.Add(9);
	listB.Add(2);

	REQUIRE(listA.Size() == 3);
	REQUIRE(listA.GetNode(0) != nullptr);
	REQUIRE(listA.GetNode(1) != nullptr);
	REQUIRE(listA.GetNode(2) != nullptr);
	REQUIRE(listA.GetNode(0)->value == 7);
	REQUIRE(listA.GetNode(1)->value == 1);
	REQUIRE(listA.GetNode(2)->value == 6);

	REQUIRE(listB.Size() == 3);
	REQUIRE(listB.GetNode(0) != nullptr);
	REQUIRE(listB.GetNode(1) != nullptr);
	REQUIRE(listB.GetNode(2) != nullptr);
	REQUIRE(listB.GetNode(0)->value == 5);
	REQUIRE(listB.GetNode(1)->value == 9);
	REQUIRE(listB.GetNode(2)->value == 2);

	DList<int> result = SumListsReverse(listA, listB);
	REQUIRE(result.IsValid());
	REQUIRE(!result.IsEmpty());
	REQUIRE(result.Size() == 3);
	REQUIRE(result.Front() != nullptr);
	REQUIRE(result.Front()->next != nullptr);
	REQUIRE(result.Front()->next->next != nullptr);
	REQUIRE(result.Front()->next->next->next == nullptr);
	REQUIRE(result.GetNode(0) != nullptr);
	REQUIRE(result.GetNode(1) != nullptr);
	REQUIRE(result.GetNode(2) != nullptr);
	REQUIRE(result.GetNode(0)->value == 2);
	REQUIRE(result.GetNode(1)->value == 1);
	REQUIRE(result.GetNode(2)->value == 9);

	//-----------------------------------------------------//

	DList<int> listA2;
	listA2.Add(6);
	listA2.Add(1);
	listA2.Add(7);

	DList<int> listB2;
	listB2.Add(2);
	listB2.Add(9);
	listB2.Add(5);

	REQUIRE(listA2.Size() == 3);
	REQUIRE(listA2.GetNode(0) != nullptr);
	REQUIRE(listA2.GetNode(1) != nullptr);
	REQUIRE(listA2.GetNode(2) != nullptr);
	REQUIRE(listA2.GetNode(0)->value == 6);
	REQUIRE(listA2.GetNode(1)->value == 1);
	REQUIRE(listA2.GetNode(2)->value == 7);

	REQUIRE(listB2.Size() == 3);
	REQUIRE(listB2.GetNode(0) != nullptr);
	REQUIRE(listB2.GetNode(1) != nullptr);
	REQUIRE(listB2.GetNode(2) != nullptr);
	REQUIRE(listB2.GetNode(0)->value == 2);
	REQUIRE(listB2.GetNode(1)->value == 9);
	REQUIRE(listB2.GetNode(2)->value == 5);

	DList<int> result2 = SumLists(listA2, listB2);
	REQUIRE(result2.IsValid());
	REQUIRE(!result2.IsEmpty());
	REQUIRE(result2.Size() == 3);
	REQUIRE(result2.Front() != nullptr);
	REQUIRE(result2.Front()->next != nullptr);
	REQUIRE(result2.Front()->next->next != nullptr);
	REQUIRE(result2.Front()->next->next->next == nullptr);
	REQUIRE(result2.GetNode(0) != nullptr);
	REQUIRE(result2.GetNode(1) != nullptr);
	REQUIRE(result2.GetNode(2) != nullptr);
	REQUIRE(result2.GetNode(0)->value == 9);
	REQUIRE(result2.GetNode(1)->value == 1);
	REQUIRE(result2.GetNode(2)->value == 2);
}

//------------------------------------------------------------//
//----------------- Palindrome (CTCI 2.6) --------------------//
//------------------------------------------------------------//

bool IsPalindrome(DList<int>::Node* head, size_t debugListSize_){
	GADGET_BASIC_ASSERT(head != nullptr);

	DList<int>::Node* cur = head;
	DList<int>::Node* skip = head->next;

	if(skip == nullptr){
		return true; //Size is 1
	}

	std::stack<int> firstHalf;

	size_t size = 1;
	while(skip != nullptr){
		firstHalf.push(cur->value);
		cur = cur->next;

		skip = skip->next;
		size++;

		if(skip != nullptr){
			skip = skip->next;
			size++;
		}
	}

	GADGET_BASIC_ASSERT(size == debugListSize_);
	GADGET_BASIC_ASSERT(cur != nullptr);
	if(size % 2 != 0){ //isOdd
		cur = cur->next; //Ignore middle
	}

	while(cur != nullptr && !firstHalf.empty()){
		if(cur->value != firstHalf.top()){
			return false;
		}

		firstHalf.pop();
		cur = cur->next;
	}

	return true;
}

TEST_CASE("DList Palindrome", "[dlist_palindrome]"){
	DList<int> list1;
	list1.Add(5);
	list1.Add(4);
	list1.Add(3);
	list1.Add(2);
	list1.Add(1);
	REQUIRE(IsPalindrome(list1.Front(), list1.Size()) == false);

	DList<int> list2;
	list2.Add(5);
	list2.Add(4);
	list2.Add(3);
	list2.Add(2);
	list2.Add(1);
	list2.Add(2);
	list2.Add(3);
	list2.Add(4);
	list2.Add(5);
	REQUIRE(IsPalindrome(list2.Front(), list2.Size()) == true);

	DList<int> list3;
	list3.Add(5);
	list3.Add(4);
	list3.Add(4);
	list3.Add(5);
	REQUIRE(IsPalindrome(list3.Front(), list3.Size()) == true);

	DList<int> list4;
	list4.Add(5);
	REQUIRE(IsPalindrome(list4.Front(), list4.Size()) == true);
}

//------------------------------------------------------------//
//---------------- Intersection (CTCI 2.7) -------------------//
//------------------------------------------------------------//

template <class T>
static inline DList<T>::Node* ListIntersects(const DList<T>& a, const DList<T>& b, size_t bListTrueSize_){
	if(a.IsEmpty() || b.IsEmpty()){
		return nullptr;
	}

	auto* aNode = a.Front();
	auto* bNode = b.Front();

	int64_t aSizeDiff = a.Size() - bListTrueSize_;
	while(aSizeDiff > 0){
		aNode = aNode->next;
		aSizeDiff--;
	}

	int64_t bSizeDiff = bListTrueSize_ - a.Size();
	while(bSizeDiff > 0){
		bNode = bNode->next;
		bSizeDiff--;
	}

	//If the lists intersect, the last node will be the same
	while(aNode->next != nullptr && bNode->next != nullptr){
		if(aNode == bNode){
			return aNode;
		}

		if(aNode->next != nullptr){
			aNode = aNode->next;
		}

		if(aNode == bNode){
			return aNode;
		}

		if(bNode->next != nullptr){
			bNode = bNode->next;
		}
	}

	if(aNode == bNode){
		return aNode;
	}

	return nullptr;
}

TEST_CASE("DList Intersection", "[dlist_intersection]"){
	//---------- Same Size ----------//
	DList<int> list1;
	list1.Add(4);
	DList<int>::Node* mid1 = list1.Add(5);
	list1.Add(6);
	list1.Add(7);

	DList<int> list2;
	list2.Add(5);
	list2.Front()->next = mid1;

	REQUIRE(ListIntersects(list1, list2, 4) == mid1);
	REQUIRE(!list2.IsValid());

	//---------- Different Sizes ----------//
	DList<float> list3;
	list3.Add(2.5f);
	list3.Add(3.5f);
	list3.Add(4.5f);
	list3.Add(5.5f);
	list3.Add(6.5f);
	DList<float>::Node* mid2 = list3.Add(7.5f);
	list3.Add(8.5f);

	DList<float> list4;
	list4.Add(0.1f);
	list4.Add(0.2f);
	list4.GetNode(1)->next = mid2;

	REQUIRE(ListIntersects(list3, list4, 4) == mid2);
	REQUIRE(!list4.IsValid());

	//---------- No Intersection ----------//
	DList<Array<int>> list5;
	list5.Add(Array<int>());
	list5.Add(Array<int>());
	list5.Add(Array<int>());
	list5.Add(Array<int>());
	list5.Add(Array<int>());

	DList<Array<int>> list6;
	list6.Add(Array<int>());
	list6.Add(Array<int>());
	list6.Add(Array<int>());
	list6.Add(Array<int>());
	list6.Add(Array<int>());

	REQUIRE(ListIntersects(list5, list6, list6.Size()) == nullptr);
	REQUIRE(list5.IsValid());
	REQUIRE(list6.IsValid());
}

//------------------------------------------------------------//
//---------------- Loop Detection (CTCI 2.8) -----------------//
//------------------------------------------------------------//

static inline DList<int>::Node* DetectLoop(DList<int>::Node* head_){
	HashTable<DList<int>::Node*, bool> table;

	DList<int>::Node* node = head_;
	while(node != nullptr){
		if(table.Contains(node)){
			return node;
		}

		table.Add(node, true);
		node = node->next;
	}

	return nullptr;
}

TEST_CASE("DList Loop Detection", "[dlist_loop_detection]"){
	//Has Loop
	DList<int> list1;
	list1.Add(1);
	list1.Add(2);
	DList<int>::Node* mid1 = list1.Add(3);
	list1.Add(4);
	list1.Add(5);

	list1.Back()->next = mid1;
	REQUIRE(!list1.IsValid());
	REQUIRE(DetectLoop(list1.Front()) == mid1);

	//No Loop
	DList<int> list2;
	list2.Add(6);
	list2.Add(7);
	list2.Add(8);
	list2.Add(9);
	REQUIRE(list2.IsValid());
	REQUIRE(DetectLoop(list2.Front()) == nullptr);
}