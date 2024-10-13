#include <Gadget.h>
#include <Core/FileSystem.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//------------------- Plain Text JSON ------------------------//
//------------------------------------------------------------//
static inline nlohmann::json JsonRW(const nlohmann::json& j_, const std::string& file_, std::string& inRaw_){
	ErrorCode err = FileSystem::WriteJSONToPlainTextFile(file_, j_, FileSystem::WriteType::Overwrite);
	REQUIRE(err == ErrorCode::OK);

	inRaw_ = FileSystem::ReadFileToString(file_);
	return FileSystem::ReadPlainTextJSONFile(file_);
}

template <int Count>
static inline void CheckEquals(const nlohmann::json& j_, const std::string& raw_, const StaticArray<nlohmann::json, Count>& jsons_, const StaticArray<std::string, Count>& raws_){
	for(int64_t i = 0; i < Count; i++){
		REQUIRE(jsons_[i] == j_);
		REQUIRE(raw_ == raws_[i]);
		REQUIRE(jsons_[i].at(0) == j_.at(0));
		REQUIRE(jsons_[i].at(1) == j_.at(1));
		REQUIRE(jsons_[i].at(2) == j_.at(2));
		REQUIRE(jsons_[i].at(3) == j_.at(3));
		REQUIRE(jsons_[i].at(4) == j_.at(4));
		REQUIRE(jsons_[i].at(5) == j_.at(5));

		if(j_.at(0).is_array()){
			size_t size = j_.at(0).size();
			for(size_t k = 0; k < size; k++){
				REQUIRE(jsons_[i].at(k).at(0) == j_.at(k).at(0));
				REQUIRE(jsons_[i].at(k).at(1) == j_.at(k).at(1));
				REQUIRE(jsons_[i].at(k).at(2) == j_.at(k).at(2));
				REQUIRE(jsons_[i].at(k).at(3) == j_.at(k).at(3));
				REQUIRE(jsons_[i].at(k).at(4) == j_.at(k).at(4));
				REQUIRE(jsons_[i].at(k).at(5) == j_.at(k).at(5));
			}
		}
	}
}

static inline void JsonRWTests(const nlohmann::json& j, const std::string& file_){
	std::string j2Text;
	nlohmann::json j2 = JsonRW(j, file_, j2Text);
	CheckEquals(j2, j2Text, StaticArray<nlohmann::json, 1>{ j }, { j2Text }); //j2Text here is intentional

	std::string j3Text;
	nlohmann::json j3 = JsonRW(j2, file_, j3Text);
	CheckEquals(j3, j3Text, StaticArray<nlohmann::json, 2>{ j, j2 }, { j2Text, j2Text });

	std::string j4Text;
	nlohmann::json j4 = JsonRW(j3, file_, j4Text);
	CheckEquals(j3, j3Text, StaticArray<nlohmann::json, 3>{ j, j2, j3 }, { j2Text, j2Text, j3Text });
}

TEST_CASE("FileSystem Plain Text JSON Read/Write" "[filesystem_plaintext_json_read_write]"){
	static constexpr const char* testJsonFileName = "Temp\\FileSystemTest_1.json";
	
	nlohmann::json j;
	j[0] = "Test";
	j[1] = "Hello";
	j[2] = "World";
	j[3] = "True";
	j[4] = true;
	j[5] = 45;

	JsonRWTests(j, testJsonFileName);

	nlohmann::json j2;
	j2[0] = j;
	j2[1] = j;
	j2[2] = j;
	j2[3] = j;
	j2[4] = j;
	j2[5] = j;

	JsonRWTests(j2, testJsonFileName);
}