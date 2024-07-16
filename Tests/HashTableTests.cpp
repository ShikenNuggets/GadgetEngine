#include <Gadget.h>
#include <Data/Array.h>
#include <Data/HashTable.h>

#include "Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//---------------- Two Sum (Leetcode 1) ----------------------//
//------------------------------------------------------------//

static inline Array<size_t> TwoSum(Array<size_t>& nums_, size_t target_){
	HashTable<size_t, size_t> table;

	for(size_t i = 0; i < nums_.Size(); i++){
		size_t complement = target_ - nums_[i];
		if(table.Contains(complement) && table[complement] != i){
			Array<size_t> result = Array<size_t>(2);
			result.Add(table[complement]);
			result.Add(i);
			return result;
		}

		table[nums_[i]] = i;
	}

	return Array<size_t>();
}

TEST_CASE("HashTable TwoSum", "[hash_table_two_sum]"){
	Array<size_t> nums;
	nums.Reserve(4);
	nums.Add(2);
	nums.Add(7);
	nums.Add(11);
	nums.Add(15);

	Array<size_t> result1 = TwoSum(nums, 9);
	REQUIRE(result1.Size() == 2);
	REQUIRE(result1[0] == 0);
	REQUIRE(result1[1] == 1);

	nums.Clear();
	nums.Add(3);
	nums.Add(2);
	nums.Add(4);

	Array<size_t> result2 = TwoSum(nums, 6);
	REQUIRE(result2.Size() == 2);
	REQUIRE(result2[0] == 1);
	REQUIRE(result2[1] == 2);

	nums.Clear();
	nums.Add(3);
	nums.Add(3);

	Array<size_t> result3 = TwoSum(nums, 6);
	REQUIRE(result3.Size() == 2);
	REQUIRE(result3[0] == 0);
	REQUIRE(result3[1] == 1);
}