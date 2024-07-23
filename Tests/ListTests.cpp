#include <Gadget.h>
#include <Data/Array.h>
#include <Data/List.h>

#include "Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//------------ Add Two Numbers (Leetcode 2) ------------------//
//------------------------------------------------------------//
static inline List<int> AddTwoNumbers(const List<int>& l1_, const List<int>& l2_){
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

	List<int> finalList;
	for(size_t i = 0; i < finalDigits.Size(); i++){
		finalList.Add(finalDigits[i]);
	}

	return finalList;
}

TEST_CASE("List - Add Two Numbers", "[list_add_two_numbers]"){
	List<int> list1;
	list1.Add(2);
	list1.Add(4);
	list1.Add(3);
	REQUIRE(list1.IsValid());

	List<int> list2;
	list2.Add(5);
	list2.Add(6);
	list2.Add(4);
	REQUIRE(list2.IsValid());

	List<int> result1 = AddTwoNumbers(list1, list2);
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

	List<int> result2 = AddTwoNumbers(list1, list2);
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

	List<int> result3 = AddTwoNumbers(list1, list2);
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
static inline void RemoveDupes1(List<int>& list_){
	List<int>::Node* current = list_.Front();

	while(current != nullptr){
		List<int>::Node* next = current->next;

		while(next != nullptr){
			if(current->value == next->value){
				List<int>::Node* oldNext = next;
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
static inline void RemoveDupes2(List<int>& list_){
	HashTable<int, int> uniques;

	List<int>::Node* current = list_.Front();
	while(current != nullptr){
		if(uniques.Contains(current->value)){
			List<int>::Node* old = current;
			current = current->next;
			list_.Remove(old);
			continue;
		}

		uniques.Add(current->value, current->value);
		current = current->next;
	}
}

TEST_CASE("List Remove Dupes", "[list_remove_dupes]"){
	List<int> list1;
	list1.Add(4);
	list1.Add(5);
	list1.Add(6);
	list1.Add(7);
	list1.Add(8);

	List<int> list1_2 = list1;

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

	List<int>::Node* current1 = list1.Front();
	List<int>::Node* current2 = list1_2.Front();
	size_t count = 0;
	while(current1 != nullptr && current2 != nullptr){
		REQUIRE(current1->value == current2->value);
		current1 = current1->next;
		current2 = current2->next;
		count++;
	}

	REQUIRE(count == 5);

	//---------------------------------//

	List<int> list2;
	list2.Add(4);
	list2.Add(4);
	list2.Add(5);
	list2.Add(4);
	list2.Add(5);

	List<int> list2_2 = list2;

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