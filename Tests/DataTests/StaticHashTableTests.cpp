#include <Gadget.h>
#include <Data/Array.h>
#include <Data/StaticHashTable.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//---------------- Two Sum (Leetcode 1) ----------------------//
//------------------------------------------------------------//

static inline Array<int64_t> TwoSum(Array<int64_t>& nums_, int64_t target_){
	StaticHashTable<int64_t, int64_t> table;

	for(int64_t i = 0; i < nums_.Size(); i++){
		int64_t complement = target_ - nums_[i];
		if(table.Contains(complement) && table[complement] != i){
			Array<int64_t> result = Array<int64_t>(2);
			result.Add(table[complement]);
			result.Add(i);
			return result;
		}

		table[nums_[i]] = i;
	}

	return Array<int64_t>();
}

TEST_CASE("StaticHashTable TwoSum", "[hash_table_two_sum]"){
	Array<int64_t> nums;
	nums.Reserve(4);
	nums.Add(2);
	nums.Add(7);
	nums.Add(11);
	nums.Add(15);

	Array<int64_t> result1 = TwoSum(nums, 9);
	REQUIRE(result1.Size() == 2);
	REQUIRE(result1[0] == 0);
	REQUIRE(result1[1] == 1);

	nums.Clear();
	nums.Add(3);
	nums.Add(2);
	nums.Add(4);

	Array<int64_t> result2 = TwoSum(nums, 6);
	REQUIRE(result2.Size() == 2);
	REQUIRE(result2[0] == 1);
	REQUIRE(result2[1] == 2);

	nums.Clear();
	nums.Add(3);
	nums.Add(3);

	Array<int64_t> result3 = TwoSum(nums, 6);
	REQUIRE(result3.Size() == 2);
	REQUIRE(result3[0] == 0);
	REQUIRE(result3[1] == 1);
}

//------------------------------------------------------------//
//-------------------- For Each Loop -------------------------//
//------------------------------------------------------------//
TEST_CASE("StaticHashTable ForEach", "[hash_table_for_each]"){
	StaticHashTable<StringID, int> values;
	values.Add(SID("Test0"), 0);
	values.Add(SID("Test1"), 1);
	values.Add(SID("Test2"), 2);
	values.Add(SID("Test3"), 3);
	values.Add(SID("Test4"), 4);
	values.Add(SID("Test5"), 5);
	values.Add(SID("Test6"), 6);
	values.Add(SID("Test7"), 7);
	values.Add(SID("Test8"), 8);
	values.Add(SID("Test9"), 9);
	values.Add(SID("Test10"), 10);
	values.Add(SID("Test11"), 11);

	REQUIRE(values.Contains(SID("Test0")));
	REQUIRE(values.Contains(SID("Test1")));
	REQUIRE(values.Contains(SID("Test2")));
	REQUIRE(values.Contains(SID("Test3")));
	REQUIRE(values.Contains(SID("Test4")));
	REQUIRE(values.Contains(SID("Test5")));
	REQUIRE(values.Contains(SID("Test6")));
	REQUIRE(values.Contains(SID("Test7")));
	REQUIRE(values.Contains(SID("Test8")));
	REQUIRE(values.Contains(SID("Test9")));
	REQUIRE(values.Contains(SID("Test10")));
	REQUIRE(values.Contains(SID("Test11")));

	REQUIRE(values[SID("Test0")] == 0);
	REQUIRE(values[SID("Test1")] == 1);
	REQUIRE(values[SID("Test2")] == 2);
	REQUIRE(values[SID("Test3")] == 3);
	REQUIRE(values[SID("Test4")] == 4);
	REQUIRE(values[SID("Test5")] == 5);
	REQUIRE(values[SID("Test6")] == 6);
	REQUIRE(values[SID("Test7")] == 7);
	REQUIRE(values[SID("Test8")] == 8);
	REQUIRE(values[SID("Test9")] == 9);
	REQUIRE(values[SID("Test10")] == 10);
	REQUIRE(values[SID("Test11")] == 11);

	size_t count = 0;
	for([[maybe_unused]] const auto& _ : values){
		count++;
	}
	REQUIRE(count > 0);

	count = 0;
	for([[maybe_unused]] auto& _ : values){
		count++;
	}
	REQUIRE(count > 0);
}