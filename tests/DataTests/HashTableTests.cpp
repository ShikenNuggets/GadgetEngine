#include <Gadget.h>
#include <Data/Array.h>
#include <Data/HashTable.h>
#include <Data/String.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//---------------- Two Sum (Leetcode 1) ----------------------//
//------------------------------------------------------------//

static inline Array<int64_t> TwoSum(Array<int64_t>& nums_, int64_t target_){
	HashTable<int64_t, int64_t> table;	REQUIRE(table.IsEmpty());

	for(int64_t i = 0; i < nums_.Size(); i++){
		REQUIRE(table.Size() == i);

		size_t complement = target_ - nums_[i];
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

TEST_CASE("HashTable TwoSum", "[hash_table_two_sum]"){
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
TEST_CASE("HashTable ForEach", "[hash_table_for_each]"){
	HashTable<StringID, int> values;
	values.Add(SID("Test0"), 0);	REQUIRE(values.Size() == 1);
	values.Add(SID("Test1"), 1);	REQUIRE(values.Size() == 2);
	values.Add(SID("Test2"), 2);	REQUIRE(values.Size() == 3);
	values.Add(SID("Test3"), 3);	REQUIRE(values.Size() == 4);
	values.Add(SID("Test4"), 4);	REQUIRE(values.Size() == 5);
	values.Add(SID("Test5"), 5);	REQUIRE(values.Size() == 6);
	values.Add(SID("Test6"), 6);	REQUIRE(values.Size() == 7);
	values.Add(SID("Test7"), 7);	REQUIRE(values.Size() == 8);
	values.Add(SID("Test8"), 8);	REQUIRE(values.Size() == 9);
	values.Add(SID("Test9"), 9);	REQUIRE(values.Size() == 10);
	values.Add(SID("Test10"), 10);	REQUIRE(values.Size() == 11);
	values.Add(SID("Test11"), 11);	REQUIRE(values.Size() == 12);

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

	int64_t count = 0;
	for([[maybe_unused]] const auto& _ : values){
		count++;
	}
	REQUIRE(count == values.Size());
	
	count = 0;
	for([[maybe_unused]] auto& _ : values){
		count++;
	}
	REQUIRE(count == values.Size());
}

//------------------------------------------------------------//
//-------- All O(1) Data Structure (Leetcode 432) ------------//
//------------------------------------------------------------//

namespace LC432{
	class AllOne{
	public:
		AllOne() = default;

		void Increment(const String& key){
			if(!data.Contains(key)){
				data[key] = 1;

				if(minValue > 1){
					minKey = key;
					minValue = 1;
				}

			} else{
				data[key]++;
				if(data[key] > maxValue){
					maxKey = key;
					maxValue = data[key];
				}
			}

			if(key == minKey){
				minKey = "";
				minValue = -1;
			}
		}

		void Decrement(const String& key){
			data[key]--;

			if(key == maxKey){
				maxKey = "";
				maxValue = -1;
			}

			if(data[key] <= 0){
				data.RemoveAt(key);
				minKey = "";
				minValue = -1;
			} else if(data[key] < minValue){
				minKey = key;
				minValue = data[key];
			}
		}

		String GetMaxKey(){
			if(maxValue > 0){
				return maxKey;
			}

			Update();
			return maxKey;
		}

		String GetMinKey(){
			if(minValue > 0){
				return minKey;
			}

			Update();
			return minKey;
		}

	private:
		HashTable<String, int> data;

		String maxKey = "";
		int maxValue = -1;

		String minKey = "";
		int minValue = -1;

		void Update(){
			maxValue = -1;
			minValue = std::numeric_limits<int>::max();
			for(const auto& [key, value] : data){
				if(value > maxValue){
					maxKey = key;
					maxValue = value;
				}

				if(value < minValue){
					minKey = key;
					minValue = value;
				}
			}
		}
	};
}

TEST_CASE("HashTable AllOne", "[hash_table_all_one]"){
	{
		LC432::AllOne data;
		data.Increment("hello");
		data.Increment("hello");
		REQUIRE(data.GetMaxKey() == "hello");
		REQUIRE(data.GetMinKey() == "hello");
		data.Increment("leet");
		REQUIRE(data.GetMaxKey() == "hello");
		REQUIRE(data.GetMinKey() == "leet");
	}
	
	{
		LC432::AllOne data;
		data.Increment("a");
		data.Increment("b");
		data.Increment("b");
		data.Increment("c");
		data.Increment("c");
		data.Increment("c");
		data.Decrement("b");
		data.Decrement("b");
		REQUIRE(data.GetMinKey() == "a");
		data.Decrement("a");
		REQUIRE(data.GetMaxKey() == "c");
		REQUIRE(data.GetMinKey() == "c");
	}

	{
		LC432::AllOne data;
		data.Increment("hello");
		data.Increment("world");
		data.Increment("hello");
		data.Decrement("world");
		data.Increment("hello");
		data.Increment("leet");
		REQUIRE(data.GetMaxKey() == "hello");
		data.Decrement("hello");
		data.Decrement("hello");
		data.Decrement("hello");
		REQUIRE(data.GetMaxKey() == "leet");
	}

	{
		LC432::AllOne data;
		data.Increment("hello");
		data.Increment("hello");
		data.Increment("world");
		data.Increment("world");
		data.Increment("hello");
		data.Decrement("world");
		REQUIRE(data.GetMaxKey() == "hello");
		REQUIRE(data.GetMinKey() == "world");
		data.Increment("world");
		data.Increment("world");
		data.Increment("leet");
		bool isValid = data.GetMaxKey() == "hello" || data.GetMaxKey() == "world";
		REQUIRE(isValid);
		REQUIRE(data.GetMinKey() == "leet");
		data.Increment("leet");
		data.Increment("leet");
		isValid = data.GetMaxKey() == "hello" || data.GetMaxKey() == "world" || data.GetMinKey() == "leet";
		REQUIRE(isValid);
	}

	{
		LC432::AllOne data;
		data.Increment("a");
		data.Increment("b");
		data.Increment("b");
		data.Increment("b");
		data.Increment("b");
		data.Decrement("b");
		data.Decrement("b");
		REQUIRE(data.GetMaxKey() == "b");
		REQUIRE(data.GetMinKey() == "a");
	}
}