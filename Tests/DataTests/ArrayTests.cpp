#include <Gadget.h>
#include <Data/Array.h>
#include <Data/String.h>

#include "_Catch2/catch_amalgamated.hpp"
#include "TestUtils/LifetimeTester.h"

using namespace Gadget;

//------------------------------------------------------------//
//------------------- Array Accessor -------------------------//
//------------------------------------------------------------//
//O(N^2) for NxN matrix
template <typename T>
void RotateMatrixRight90(Array<Array<T>>& inMatrix_){
	if(inMatrix_.Size() <= 1 || inMatrix_.Size() != inMatrix_[0].Size()){
		return;
	}

	const int64_t n = inMatrix_.Size();
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

TEST_CASE("Array Accessor" "[array_access]"){
	//----- 2x2 rotation -----//
	Array<Array<int>> matrix;
	matrix.Add(Array<int>());
	matrix.Add(Array<int>());

	matrix[0].Add(0);
	matrix[0].Add(1);
	matrix[1].Add(2);
	matrix[1].Add(3);

	REQUIRE(matrix.Size() == 2);
	REQUIRE(matrix.Capacity() >= 2);
	REQUIRE(matrix[0].Size() == 2);
	REQUIRE(matrix[0].Capacity() >= 2);
	REQUIRE(matrix[1].Size() == 2);
	REQUIRE(matrix[1].Capacity() >= 2);

	matrix.ShrinkToFit();
	matrix[0].ShrinkToFit();
	matrix[1].ShrinkToFit();

	REQUIRE(matrix.Size() == matrix.Capacity());
	REQUIRE(matrix[0].Size() == matrix[0].Capacity());
	REQUIRE(matrix[1].Size() == matrix[1].Capacity());

	REQUIRE(matrix[0][0] == 0);
	REQUIRE(matrix[0][1] == 1);
	REQUIRE(matrix[1][0] == 2);
	REQUIRE(matrix[1][1] == 3);

	RotateMatrixRight90(matrix);

	REQUIRE(matrix[0][0] == 2);
	REQUIRE(matrix[0][1] == 0);
	REQUIRE(matrix[1][0] == 3);
	REQUIRE(matrix[1][1] == 1);

	//----- 3x3 rotation -----//
	matrix.Clear();
	matrix.Reserve(3);
	matrix.Add(Array<int>(3));
	matrix.Add(Array<int>(3));
	matrix.Add(Array<int>(3));

	matrix[0].Add(0);
	matrix[0].Add(1);
	matrix[0].Add(2);
	matrix[1].Add(3);
	matrix[1].Add(4);
	matrix[1].Add(5);
	matrix[2].Add(6);
	matrix[2].Add(7);
	matrix[2].Add(8);

	REQUIRE(matrix.Size() == 3);
	REQUIRE(matrix[0].Size() == 3);
	REQUIRE(matrix[1].Size() == 3);
	REQUIRE(matrix[2].Size() == 3);

	REQUIRE(matrix.Size() == matrix.Capacity());
	REQUIRE(matrix[0].Size() == matrix[0].Capacity());
	REQUIRE(matrix[1].Size() == matrix[1].Capacity());
	REQUIRE(matrix[2].Size() == matrix[2].Capacity());
	
	REQUIRE(matrix[0][0] == 0);
	REQUIRE(matrix[0][1] == 1);
	REQUIRE(matrix[0][2] == 2);
	REQUIRE(matrix[1][0] == 3);
	REQUIRE(matrix[1][1] == 4);
	REQUIRE(matrix[1][2] == 5);
	REQUIRE(matrix[2][0] == 6);
	REQUIRE(matrix[2][1] == 7);
	REQUIRE(matrix[2][2] == 8);

	RotateMatrixRight90(matrix);

	REQUIRE(matrix[0][0] == 6);
	REQUIRE(matrix[0][1] == 3);
	REQUIRE(matrix[0][2] == 0);
	REQUIRE(matrix[1][0] == 7);
	REQUIRE(matrix[1][1] == 4);
	REQUIRE(matrix[1][2] == 1);
	REQUIRE(matrix[2][0] == 8);
	REQUIRE(matrix[2][1] == 5);
	REQUIRE(matrix[2][2] == 2);
}

//------------------------------------------------------------//
//------------- Array Construction/Assignment ----------------//
//------------------------------------------------------------//
TEST_CASE("Array Construction/Assignment" "[array_construct_assign]"){
	//Default construction
	Array<int> test1;
	test1.Add(0);
	test1.Add(1);
	test1.Add(2);
	test1.Add(3);
	test1.Add(4);
	REQUIRE(test1.Size() == 5);
	for(int64_t i = 0; i < test1.Size(); i++){
		REQUIRE(test1[i] == i);
	}

	//Copy construction
	Array<int> test2(test1);
	REQUIRE(test1.Size() == test2.Size());
	for(int64_t i = 0; i < test1.Size(); i++){
		REQUIRE(test1[i] == test2[i]);
	}

	//Move construction
	Array<int> test3(std::move(test2));
	REQUIRE(test3.Size() == test1.Size());
	REQUIRE(test2.IsEmpty());
	for(int64_t i = 0; i < test1.Size(); i++){
		REQUIRE(test1[i] == test3[i]);
	}

	//Copy assignment
	Array<int> test4 = test1;
	REQUIRE(test1.Size() == test4.Size());
	for(int64_t i = 0; i < test4.Size(); i++){
		REQUIRE(test1[i] == test4[i]);
	}

	//Move assignment
	Array<int> test5 = std::move(test4);
	REQUIRE(test5.Size() == test1.Size());
	REQUIRE(test4.IsEmpty());
	for(int64_t i = 0; i < test1.Size(); i++){
		REQUIRE(test1[i] == test5[i]);
	}
}

//------------------------------------------------------------//
//--------------------- Array Emplace ------------------------//
//------------------------------------------------------------//
TEST_CASE("Array Emplace", "[array_emplace]"){
	{
		Array<LifetimeTester> array1;	REQUIRE(array1.Size() == 0);
		array1.Emplace(1, 2, "Hello");	REQUIRE(array1.Size() == 1);	REQUIRE(array1[0].GetA() == 1);	REQUIRE(LifetimeTester::numConstructs == 1);
		array1.Emplace(3, 4, "World");	REQUIRE(array1.Size() == 2);	REQUIRE(array1[1].GetA() == 3);	REQUIRE(LifetimeTester::numConstructs == 2);
		array1.Emplace(5, 6, "!");		REQUIRE(array1.Size() == 3);	REQUIRE(array1[2].GetA() == 5);	REQUIRE(LifetimeTester::numConstructs == 3);

		REQUIRE(LifetimeTester::numDestructs == 0);
		REQUIRE(LifetimeTester::numCopies == 0);
		REQUIRE(LifetimeTester::numMoves == 0);
	}

	REQUIRE(LifetimeTester::numDestructs == LifetimeTester::numConstructs);
	LifetimeTester::Reset();
}

//------------------------------------------------------------//
//--------------- Zero Matrix (CTCI 1.8) ---------------------//
//------------------------------------------------------------//
//O(rows * cols)
template <typename T>
static inline void ZeroMatrix(Array<Array<T>>& inMatrix_){
	Array<int64_t> rows;
	Array<int64_t> cols;

	for(int64_t i = 0; i < inMatrix_.Size(); i++){
		for(int64_t j = 0; j < inMatrix_[i].Size(); j++){
			if(inMatrix_[i][j] == 0){
				rows.Add(j);
				cols.Add(i);
			}
		}
	}

	for(int64_t i = 0; i < rows.Size(); i++){
		for(int64_t j = 0; j < inMatrix_.Size(); j++){
			inMatrix_[j][rows[i]] = 0;
		}
	}

	for(int64_t i = 0; i < cols.Size(); i++){
		for(int64_t j = 0; j < inMatrix_[cols[i]].Size(); j++){
			inMatrix_[cols[i]][j] = 0;
		}
	}
}

TEST_CASE("Array Zero Matrix", "[array_zero_matrix]"){
	Array<Array<int>> matrix;
	matrix.Add(Array<int>());
	matrix.Add(Array<int>());
	matrix.Add(Array<int>());

	matrix[0].Add(1);
	matrix[0].Add(1);
	matrix[0].Add(1);
	matrix[0].Add(1);

	matrix[1].Add(0);
	matrix[1].Add(1);
	matrix[1].Add(0);
	matrix[1].Add(1);

	matrix[2].Add(1);
	matrix[2].Add(1);
	matrix[2].Add(1);
	matrix[2].Add(1);

	REQUIRE(matrix.Size() == 3);
	REQUIRE(matrix[0].Size() == 4);
	REQUIRE(matrix[1].Size() == 4);
	REQUIRE(matrix[2].Size() == 4);

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

	REQUIRE(matrix.Size() == 3);
	REQUIRE(matrix[0].Size() == 4);
	REQUIRE(matrix[1].Size() == 4);
	REQUIRE(matrix[2].Size() == 4);

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
	Person() : name(""), height(0){}
	Person(const String& name_, int height_) : name(name_), height(height_){}

	String name;
	int height;

	constexpr bool operator<(const Person& other_){ return height < other_.height; }
	constexpr bool operator>(const Person& other_){ return height > other_.height; }
};

//O(n log n)
static inline Array<String> SortPeopleByHeight(const Array<String>& names_, const Array<int> heights_){
	GADGET_BASIC_ASSERT(names_.Size() == heights_.Size());
	Array<Person> people = Array<Person>(names_.Size());
	for(int64_t i = 0; i < names_.Size(); i++){
		people.Add(Person(names_[i], heights_[i]));
	}

	people.QuickSort();

	Array<String> finalNames;
	for(const Person& p : people){
		finalNames.Add(p.name);
	}

	return finalNames;
}

TEST_CASE("Array Sort the People", "[array_sort_the_people]"){
	Array<String> names1 = Array<String>(3);
	names1.Add("Mary");
	names1.Add("John");
	names1.Add("Emma");

	Array<int> heights1 = Array<int>(3);
	heights1.Add(180);
	heights1.Add(165);
	heights1.Add(170);

	REQUIRE(names1.Size() == heights1.Size());
	Array<String> sorted1 = SortPeopleByHeight(names1, heights1);
	REQUIRE(sorted1.Size() == names1.Size());
	REQUIRE(sorted1[0] == "John");
	REQUIRE(sorted1[1] == "Emma");
	REQUIRE(sorted1[2] == "Mary");

	//-----------------------------------------//

	Array<String> names2 = Array<String>(3);
	names2.Add("Alice");
	names2.Add("Bob");
	names2.Add("Bob");

	Array<int> heights2 = Array<int>(3);
	heights2.Add(155);
	heights2.Add(185);
	heights2.Add(150);

	REQUIRE(names2.Size() == heights2.Size());
	Array<String> sorted2 = SortPeopleByHeight(names2, heights2);
	REQUIRE(sorted2.Size() == names2.Size());
	REQUIRE(sorted2[0] == "Bob");
	REQUIRE(sorted2[1] == "Alice");
	REQUIRE(sorted2[2] == "Bob");
}

//------------------------------------------------------------//
//------------ Lemonade Change (Leetcode 860) ----------------//
//------------------------------------------------------------//

static inline bool LemonadeChange(const Array<int>& bills){
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
			}else{
				bill5 -= 3;
			}
		}

		if(bill5 < 0 || bill10 < 0){
			return false;
		}
	}

	return true;
}

TEST_CASE("Array Lemonade Change", "[array_lemonade_change]"){
	Array<int> case1;
	case1.Add(5);
	case1.Add(5);
	case1.Add(5);
	case1.Add(10);
	case1.Add(20);

	Array<int> case2;
	case2.Add(5);
	case2.Add(5);
	case2.Add(10);
	case2.Add(10);
	case2.Add(20);

	Array<int> case3;
	case3.Add(5);
	case3.Add(5);
	case3.Add(10);
	case3.Add(20);
	case3.Add(5);
	case3.Add(5);
	case3.Add(5);
	case3.Add(5);
	case3.Add(5);
	case3.Add(5);
	case3.Add(5);
	case3.Add(5);
	case3.Add(5);
	case3.Add(10);
	case3.Add(5);
	case3.Add(5);
	case3.Add(20);
	case3.Add(5);
	case3.Add(20);
	case3.Add(5);

	REQUIRE(LemonadeChange(case1) == true);
	REQUIRE(LemonadeChange(case2) == false);
	REQUIRE(LemonadeChange(case3) == true);
}

//------------------------------------------------------------//
//--------- Semi-Ordered Permutation (Leetcode 2717) ---------//
//------------------------------------------------------------//

static inline int64_t SemiOrderedPermutation(const Array<int>& nums){
	if(nums.Size() <= 1){
		return 0;
	}

	int64_t smallIdx = -1;
	int64_t bigIdx = -1;
	int bigNum = -1;

	for(int64_t i = 0; i < nums.Size(); i++){
		if(nums[i] == 1){
			smallIdx = i;
		}

		if(nums[i] > bigNum){
			bigIdx = i;
			bigNum = nums[i];
		}
	}

	int64_t numMoves = smallIdx + (nums.Size() - 1 - bigIdx);
	if(bigIdx < smallIdx){
		//We get a swap for free
		numMoves--;
	}

	return numMoves;
}

TEST_CASE("Array Semi-Ordered Permutation", "[array_semi_ordered_permuation]"){
	Array<int> test1;	 REQUIRE(test1.IsEmpty());
	test1.Add(2);		 REQUIRE(test1.Size() == 1);	REQUIRE(test1[0] == 2);
	test1.Add(1);		 REQUIRE(test1.Size() == 2);	REQUIRE(test1[1] == 1);
	test1.Add(4);		 REQUIRE(test1.Size() == 3);	REQUIRE(test1[2] == 4);
	test1.Add(3);		 REQUIRE(test1.Size() == 4);	REQUIRE(test1[3] == 3);
	test1.ShrinkToFit(); REQUIRE(test1.Size() == test1.Capacity());
	REQUIRE(SemiOrderedPermutation(test1) == 2);

	Array<int> test2;	 REQUIRE(test2.IsEmpty());
	test2.Add(2);		 REQUIRE(test2.Size() == 1);	REQUIRE(test2[0] == 2);
	test2.Add(4);		 REQUIRE(test2.Size() == 2);	REQUIRE(test2[1] == 4);
	test2.Add(1);		 REQUIRE(test2.Size() == 3);	REQUIRE(test2[2] == 1);
	test2.Add(3);		 REQUIRE(test2.Size() == 4);	REQUIRE(test2[3] == 3);
	test2.ShrinkToFit(); REQUIRE(test2.Size() == test2.Capacity());
	REQUIRE(SemiOrderedPermutation(test2) == 3);

	Array<int> test3;	 REQUIRE(test3.IsEmpty());
	test3.Add(1);		 REQUIRE(test3.Size() == 1);	REQUIRE(test3[0] == 1);
	test3.Add(3);		 REQUIRE(test3.Size() == 2);	REQUIRE(test3[1] == 3);
	test3.Add(4);		 REQUIRE(test3.Size() == 3);	REQUIRE(test3[2] == 4);
	test3.Add(2);		 REQUIRE(test3.Size() == 4);	REQUIRE(test3[3] == 2);
	test3.Add(5);		 REQUIRE(test3.Size() == 5);	REQUIRE(test3[4] == 5);
	test3.ShrinkToFit(); REQUIRE(test3.Size() == test3.Capacity());
	REQUIRE(SemiOrderedPermutation(test3) == 0);
}