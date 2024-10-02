#include <Gadget.h>
#include <Data/StaticArray.h>
#include <Data/String.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//------------------ StaticArray Accessor --------------------//
//------------------------------------------------------------//
//O(N^2) for NxN matrix
template <typename T, int Size>
void RotateMatrixRight90(StaticArray<StaticArray<T, Size>, Size>& inMatrix_){
	if constexpr(Size <= 1){
		return;
	}

	const int64_t n = Size;
	for(int64_t layer = 0; layer < n / 2; layer++){
		int64_t first = layer;
		int64_t last = n - 1 - layer;

		for(int64_t i = first; i < last; i++){
			int64_t offset = i - first;
			T top = inMatrix_[first][i];
			inMatrix_[first][i] = inMatrix_[last - offset][first];
			inMatrix_[last - offset][first] = inMatrix_[last][last - offset];
			inMatrix_[last][last - offset] = inMatrix_[i][last];
			inMatrix_[i][last] = top;
		}
	}
}

TEST_CASE("StaticArray Accessor" "[static_array_access]"){
	//----- 2x2 rotation -----//
	StaticArray<StaticArray<int, 2>, 2> matrix2{};
	matrix2[0][0] = 0;
	matrix2[0][1] = 1;
	matrix2[1][0] = 2;
	matrix2[1][1] = 3;

	REQUIRE(matrix2[0][0] == 0);
	REQUIRE(matrix2[0][1] == 1);
	REQUIRE(matrix2[1][0] == 2);
	REQUIRE(matrix2[1][1] == 3);

	RotateMatrixRight90(matrix2);

	REQUIRE(matrix2[0][0] == 2);
	REQUIRE(matrix2[0][1] == 0);
	REQUIRE(matrix2[1][0] == 3);
	REQUIRE(matrix2[1][1] == 1);

	//----- 3x3 rotation -----//
	StaticArray<StaticArray<int, 3>, 3> matrix3{};
	matrix3[0][0] = 0;
	matrix3[0][1] = 1;
	matrix3[0][2] = 2;
	matrix3[1][0] = 3;
	matrix3[1][1] = 4;
	matrix3[1][2] = 5;
	matrix3[2][0] = 6;
	matrix3[2][1] = 7;
	matrix3[2][2] = 8;

	REQUIRE(matrix3[0][0] == 0);
	REQUIRE(matrix3[0][1] == 1);
	REQUIRE(matrix3[0][2] == 2);
	REQUIRE(matrix3[1][0] == 3);
	REQUIRE(matrix3[1][1] == 4);
	REQUIRE(matrix3[1][2] == 5);
	REQUIRE(matrix3[2][0] == 6);
	REQUIRE(matrix3[2][1] == 7);
	REQUIRE(matrix3[2][2] == 8);

	RotateMatrixRight90(matrix3);

	REQUIRE(matrix3[0][0] == 6);
	REQUIRE(matrix3[0][1] == 3);
	REQUIRE(matrix3[0][2] == 0);
	REQUIRE(matrix3[1][0] == 7);
	REQUIRE(matrix3[1][1] == 4);
	REQUIRE(matrix3[1][2] == 1);
	REQUIRE(matrix3[2][0] == 8);
	REQUIRE(matrix3[2][1] == 5);
	REQUIRE(matrix3[2][2] == 2);
}

//------------------------------------------------------------//
//----------- StaticArray Construction/Assignment ------------//
//------------------------------------------------------------//
TEST_CASE("StaticArray Construction/Assignment" "[staticarray_construct_assign]"){
	static constexpr StaticArray<int, 5> test1 = { 0, 1, 2, 3, 4 };
	REQUIRE(test1.Size() == 5);
	for(int64_t i = 0; i < test1.Size(); i++){
		REQUIRE(test1[i] == i);
	}

	//Copy construction
	StaticArray<int, 5> test2(test1);
	REQUIRE(test1.Size() == test2.Size());
	for(int64_t i = 0; i < test1.Size(); i++){
		REQUIRE(test1[i] == test2[i]);
	}

	//Move construction
	StaticArray<int, 5> test3(std::move(test2));
	REQUIRE(test3.Size() == test1.Size());
	for(int64_t i = 0; i < test1.Size(); i++){
		REQUIRE(test1[i] == test3[i]);
	}

	//Copy assignment
	StaticArray<int, 5> test4 = test1;
	REQUIRE(test1.Size() == test4.Size());
	for(int64_t i = 0; i < test4.Size(); i++){
		REQUIRE(test1[i] == test4[i]);
	}

	//Move assignment
	StaticArray<int, 5> test5 = std::move(test4);
	REQUIRE(test5.Size() == test1.Size());
	for(int64_t i = 0; i < test1.Size(); i++){
		REQUIRE(test1[i] == test5[i]);
	}
}

//------------------------------------------------------------//
//--------------- Zero Matrix (CTCI 1.8) ---------------------//
//------------------------------------------------------------//
//O(rows * cols)
template <typename T, int SizeR, int SizeC>
static inline void ZeroMatrix(StaticArray<StaticArray<T, SizeC>, SizeR>& inMatrix_){
	Array<int64_t> rows;
	Array<int64_t> cols;

	for(int64_t i = 0; i < SizeR; i++){
		for(int64_t j = 0; j < SizeC; j++){
			if(inMatrix_[i][j] == 0){
				rows.Add(j);
				cols.Add(i);
			}
		}
	}

	for(int64_t i = 0; i < rows.Size(); i++){
		for(int64_t j = 0; j < SizeR; j++){
			inMatrix_[j][rows[i]] = 0;
		}
	}

	for(int64_t i = 0; i < cols.Size(); i++){
		for(int64_t j = 0; j < SizeC; j++){
			inMatrix_[cols[i]][j] = 0;
		}
	}
}

TEST_CASE("StaticArray Zero Matrix", "[static_array_zero_matrix]"){
	StaticArray<StaticArray<int, 4>, 3> matrix{};
	matrix[0][0] = 1;
	matrix[0][1] = 1;
	matrix[0][2] = 1;
	matrix[0][3] = 1;

	matrix[1][0] = 0;
	matrix[1][1] = 1;
	matrix[1][2] = 0;
	matrix[1][3] = 1;

	matrix[2][0] = 1;
	matrix[2][1] = 1;
	matrix[2][2] = 1;
	matrix[2][3] = 1;

	REQUIRE(matrix[0][0] == 1);
	REQUIRE(matrix[0][1] == 1);
	REQUIRE(matrix[0][2] == 1);
	REQUIRE(matrix[0][3] == 1);

	REQUIRE(matrix[1][0] == 0);
	REQUIRE(matrix[1][1] == 1);
	REQUIRE(matrix[1][2] == 0);
	REQUIRE(matrix[1][3] == 1);

	REQUIRE(matrix[2][0] == 1);
	REQUIRE(matrix[2][1] == 1);
	REQUIRE(matrix[2][2] == 1);
	REQUIRE(matrix[2][3] == 1);

	ZeroMatrix(matrix);

	REQUIRE(matrix[0][0] == 0);
	REQUIRE(matrix[0][1] == 1);
	REQUIRE(matrix[0][2] == 0);
	REQUIRE(matrix[0][3] == 1);

	REQUIRE(matrix[1][0] == 0);
	REQUIRE(matrix[1][1] == 0);
	REQUIRE(matrix[1][2] == 0);
	REQUIRE(matrix[1][3] == 0);

	REQUIRE(matrix[2][0] == 0);
	REQUIRE(matrix[2][1] == 1);
	REQUIRE(matrix[2][2] == 0);
	REQUIRE(matrix[2][3] == 1);
}

//------------------------------------------------------------//
//----------- Sort the People (Leetcode 2418) ----------------//
//------------------------------------------------------------//
struct Person{
	Person(){}
	Person(const String& name_, int height_) : name(name_), height(height_){}

	String name;
	int height;

	constexpr bool operator<(const Person& other_) const{ return height < other_.height; }
	constexpr bool operator>(const Person& other_) const{ return height > other_.height; }
};

//O(n log n)
template <int NumPeople>
static inline StaticArray<String, NumPeople> SortPeopleByHeight(const StaticArray<String, NumPeople>& names_, const StaticArray<int, NumPeople> heights_){
	StaticArray<Person, NumPeople> people;
	for(int64_t i = 0; i < NumPeople; i++){
		people[i] = Person(names_[i], heights_[i]);
	}

	people.QuickSort();

	StaticArray<String, NumPeople> finalNames;
	for(int64_t i = 0; i < NumPeople; i++){
		finalNames[i] = people[i].name;
	}

	return finalNames;
}

TEST_CASE("StaticArray Sort the People", "[staticarray_sort_the_people]"){
	StaticArray<String, 3> names1;
	names1[0] = "Mary";
	names1[1] = "John";
	names1[2] = "Emma";

	StaticArray<int, 3> heights1{};
	heights1[0] = 180;
	heights1[1] = 165;
	heights1[2] = 170;

	REQUIRE(names1.Size() == heights1.Size());
	StaticArray<String, 3> sorted1 = SortPeopleByHeight(names1, heights1);
	REQUIRE(sorted1.Size() == names1.Size());
	REQUIRE(sorted1[0] == "John");
	REQUIRE(sorted1[1] == "Emma");
	REQUIRE(sorted1[2] == "Mary");

	//-----------------------------------------//

	StaticArray<String, 3> names2;
	names2[0] = "Alice";
	names2[1] = "Bob";
	names2[2] = "Bob";

	StaticArray<int, 3> heights2{};
	heights2[0] = 155;
	heights2[1] = 185;
	heights2[2] = 150;

	REQUIRE(names2.Size() == heights2.Size());
	StaticArray<String, 3> sorted2 = SortPeopleByHeight(names2, heights2);
	REQUIRE(sorted2.Size() == names2.Size());
	REQUIRE(sorted2[0] == "Bob");
	REQUIRE(sorted2[1] == "Alice");
	REQUIRE(sorted2[2] == "Bob");
}

//------------------------------------------------------------//
//------------ Lemonade Change (Leetcode 860) ----------------//
//------------------------------------------------------------//
template <int Size>
static inline bool LemonadeChange(const StaticArray<int, Size>& bills){
	int bill5 = 0;
	int bill10 = 0;

	for(const auto& b : bills){
		if(b == 5){
			bill5++;
			continue;
		}

		if(b == 10){
			bill10++;
			bill5--;
		}else if(b == 20){
			if(bill10 > 0){
				bill10--;
				bill5--;
			} else{
				bill5 -= 3;
			}
		}

		if(bill5 < 0 || bill10 < 0){
			return false;
		}
	}

	return true;
}

TEST_CASE("StaticArray Lemonade Change", "[staticarray_lemonade_change]"){
	StaticArray<int, 5> case1{};
	case1[0] = 5;
	case1[1] = 5;
	case1[2] = 5;
	case1[3] = 10;
	case1[4] = 20;

	StaticArray<int, 5> case2{};
	case2[0] = 5;
	case2[1] = 5;
	case2[2] = 10;
	case2[3] = 10;
	case2[4] = 20;

	StaticArray<int, 20> case3{};
	case3[0] = 5;
	case3[1] = 5;
	case3[2] = 10;
	case3[3] = 20;
	case3[4] = 5;
	case3[5] = 5;
	case3[6] = 5;
	case3[7] = 5;
	case3[8] = 5;
	case3[9] = 5;
	case3[10] = 5;
	case3[11] = 5;
	case3[12] = 5;
	case3[13] = 10;
	case3[14] = 5;
	case3[15] = 5;
	case3[16] = 20;
	case3[17] = 5;
	case3[18] = 20;
	case3[19] = 5;

	REQUIRE(LemonadeChange(case1) == true);
	REQUIRE(LemonadeChange(case2) == false);
	REQUIRE(LemonadeChange(case3) == true);
}

//------------------------------------------------------------//
//--------- Semi-Ordered Permutation (Leetcode 2717) ---------//
//------------------------------------------------------------//

template<int _Capacity>
static inline int64_t SemiOrderedPermutation(const StaticArray<int, _Capacity>& nums){
	if constexpr(_Capacity <= 1){
		return 0;
	}

	int64_t smallIdx = -1;
	int64_t bigIdx = -1;
	int bigNum = -1;

	for(int64_t i = 0; i < _Capacity; i++){
		if(nums[i] == 1){
			smallIdx = i;
		}

		if(nums[i] > bigNum){
			bigIdx = i;
			bigNum = nums[i];
		}
	}

	int64_t numMoves = smallIdx + (_Capacity - 1 - bigIdx);
	if(bigIdx < smallIdx){
		//We get a swap for free
		numMoves--;
	}

	return numMoves;
}

TEST_CASE("StaticArray Semi-Ordered Permutation", "[staticarray_semi_ordered_permuation]"){
	StaticArray<int, 4> test1{};
	test1[0] = 2;
	test1[1] = 1;
	test1[2] = 4;
	test1[3] = 3;
	REQUIRE(SemiOrderedPermutation(test1) == 2);

	StaticArray<int, 4> test2{};
	test2[0] = 2;
	test2[1] = 4;
	test2[2] = 1;
	test2[3] = 3;
	REQUIRE(SemiOrderedPermutation(test2) == 3);

	StaticArray<int, 5> test3{};
	test3[0] = 1;
	test3[1] = 3;
	test3[2] = 4;
	test3[3] = 2;
	test3[4] = 5;
	REQUIRE(SemiOrderedPermutation(test3) == 0);
}

//------------------------------------------------------------//
//---- Maximum Value of an Ordered Triplet (Leetcode 2873) ---//
//------------------------------------------------------------//

template <int Size>
static constexpr inline int64_t MaximumTripletValue(const StaticArray<int, Size>& nums_){
	int64_t maxTriplet = 0;

	for(int64_t i = 0; i < nums_.Size(); i++){
		for(int64_t j = i + 1; j < nums_.Size(); j++){
			for(int64_t k = j + 1; k < nums_.Size(); k++){
				int64_t value = static_cast<int64_t>(nums_[i] - nums_[j]) * nums_[k];
				if(value > maxTriplet){
					maxTriplet = value;
				}
			}
		}
	}

	return maxTriplet;
}

TEST_CASE("StaticArray Maximum Value of an Ordered Triplet", "[staticarray_max_ordered_triplet]"){
	StaticArray<int, 5> nums = { 12, 6, 1, 2, 7 };
	REQUIRE(MaximumTripletValue(nums) == 77);

	nums = { 1, 10, 3, 4, 19 };
	REQUIRE(MaximumTripletValue(nums) == 133);

	constexpr StaticArray<int, 3> nums3 = { 1, 2, 3 };
	REQUIRE(MaximumTripletValue(nums3) == 0);
}

//------------------------------------------------------------//
//-------- Find the Key of the Numbers (Leetcode 3270) -------//
//------------------------------------------------------------//

template <int NumDigits>
static inline StaticArray<int, NumDigits> GetDigits(int num_){
	static_assert(NumDigits > 0);
	GADGET_BASIC_ASSERT(num_ >= 0); //This only makes sense for positive integers
	StaticArray<int, NumDigits> result{};
	for(int64_t i = result.Size() - 1; i >= 0; i--){
		result[i] = num_ % 10;
		num_ /= 10;
	}

	return result;
}

static inline int GenerateKey(int num1_, int num2_, int num3_){
	auto digits1 = GetDigits<4>(num1_);
	auto digits2 = GetDigits<4>(num2_);
	auto digits3 = GetDigits<4>(num3_);

	StaticArray<int, 4> keyDigits{
		std::min({ digits1[0], digits2[0], digits3[0] }),
		std::min({ digits1[1], digits2[1], digits3[1] }),
		std::min({ digits1[2], digits2[2], digits3[2] }),
		std::min({ digits1[3], digits2[3], digits3[3] }),
	};

	return (keyDigits[0] * 1000) + (keyDigits[1] * 100) + (keyDigits[2] * 10) + keyDigits[3];
}

TEST_CASE("StaticArray Find Key of the Numbers", "[staticarray_find_key]"){
	REQUIRE(GenerateKey(1, 10, 1000) == 0);
	REQUIRE(GenerateKey(987, 879, 798) == 777);
	REQUIRE(GenerateKey(1, 2, 3) == 1);
}