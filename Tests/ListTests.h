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
			newDigit += l2Digits.Size();
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
		finalList.AddFront(finalDigits[i]);
	}

	return finalList;
}

TEST_CASE("List - Add Two Numbers", "[list_add_two_numbers]"){
	List<int> list1;
	list1.Add(2);
	list1.Add(4);
	list1.Add(3);

	List<int> list2;
	list2.Add(5);
	list2.Add(6);
	list2.Add(4);

	List<int> result1 = AddTwoNumbers(list1, list2);
	REQUIRE(result1.IsEmpty() == false);
	REQUIRE(result1.Front() != nullptr);
	REQUIRE(result1.Front()->next != nullptr);
	REQUIRE(result1.Front()->next->next != nullptr);
	REQUIRE(result1.Front()->value == 7);
	REQUIRE(result1.Front()->next->value == 7);
	REQUIRE(result1.Front()->next->next->value == 7);

	list1.Clear();
	list1.Add(0);

	list2.Clear();
	list2.Add(0);

	List<int> result2 = AddTwoNumbers(list1, list2);
	REQUIRE(result1.IsEmpty() == false);
	REQUIRE(result1.Front() != nullptr);
	REQUIRE(result1.Front()->value == 0);

	list1.Clear();
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);
	list1.Add(9);

	list2.Clear();
	list2.Add(9);
	list2.Add(9);
	list2.Add(9);
	list2.Add(9);

	List<int> result2 = AddTwoNumbers(list1, list2);
	REQUIRE(result1.IsEmpty() == false);
	REQUIRE(result1.Front() != nullptr);
	REQUIRE(result1.Front()->next != nullptr);
	REQUIRE(result1.Front()->next->next != nullptr);
	REQUIRE(result1.Front()->next->next->next != nullptr);
	REQUIRE(result1.Front()->next->next->next != nullptr);
	REQUIRE(result1.Front()->next->next->next->next != nullptr);
	REQUIRE(result1.Front()->next->next->next->next->next != nullptr);
	REQUIRE(result1.Front()->next->next->next->next->next->next != nullptr);

	REQUIRE(result1.Front()->value == 0);
	REQUIRE(result1.Front()->next->value == 0);
	REQUIRE(result1.Front()->next->next->value == 0);
	REQUIRE(result1.Front()->next->next->next->value == 0);
	REQUIRE(result1.Front()->next->next->next->next->value == 0);
	REQUIRE(result1.Front()->next->next->next->next->next->value == 0);
	REQUIRE(result1.Front()->next->next->next->next->next->next->value == 0);
	REQUIRE(result1.Front()->next->next->next->next->next->next->next->value == 0);
}