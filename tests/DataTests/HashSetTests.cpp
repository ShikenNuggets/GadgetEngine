#include <Gadget.h>
#include <Data/Array.h>
#include <Data/HashSet.h>
#include <Data/StaticArray.h>
#include <Data/String.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//-------------------- For Each Loop -------------------------//
//------------------------------------------------------------//
TEST_CASE("HashSet ForEach", "[hash_set_for_each]"){
	HashSet<StringID> values;
	values.Add(SID("Test0"));
	values.Add(SID("Test1"));
	values.Add(SID("Test2"));
	values.Add(SID("Test3"));
	values.Add(SID("Test4"));
	values.Add(SID("Test5"));
	values.Add(SID("Test6"));
	values.Add(SID("Test7"));
	values.Add(SID("Test8"));
	values.Add(SID("Test9"));
	values.Add(SID("Test10"));
	values.Add(SID("Test11"));

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

	int64_t count = 0;
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

//------------------------------------------------------------//
//----------- Destination City (Leetcode 1436) ---------------//
//------------------------------------------------------------//

static inline String DestinationCity(const Array<StaticArray<String, 2>>& paths){
	HashSet<String> starts;

	for(const auto& p : paths){
		starts.Add(p[0]);
	}

	for(const auto& p : paths){
		if(!starts.Contains(p[1])){
			return p[1];
		}
	}

	GADGET_ASSERT_UNREACHABLE;
	return "";
}

TEST_CASE("HashSet Destination City", "[hash_set_destination_city]"){
	Array<StaticArray<String, 2>> data;
	data.Add({ "London", "New York" });
	data.Add({ "New York", "Lima" });
	data.Add({ "Lima", "Sao Paulo" });
	REQUIRE(DestinationCity(data) == "Sao Paulo");

	data.Clear();
	data.Add({ "B", "C" });
	data.Add({ "D", "B" });
	data.Add({ "C", "A" });
	REQUIRE(DestinationCity(data) == "A");

	data.Clear();
	data.Add({ "A", "Z" });
	REQUIRE(DestinationCity(data) == "Z");
}