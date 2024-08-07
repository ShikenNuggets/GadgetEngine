#include <Gadget.h>
#include <Data/StaticArray.h>
#include <Data/String.h>

#include "Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//------------------ StaticArray Accessor --------------------//
//------------------------------------------------------------//
//O(N^2) for NxN matrix
template <typename T, int Size>
void RotateMatrixRight90(StaticArray<StaticArray<T, Size>, Size>& inMatrix_){
	if(Size <= 1){
		return;
	}

	const size_t n = Size;
	for(size_t layer = 0; layer < n / 2; layer++){
		size_t first = layer;
		size_t last = n - 1 - layer;

		for(size_t i = first; i < last; i++){
			size_t offset = i - first;
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
	StaticArray<StaticArray<int, 2>, 2> matrix2;
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
	StaticArray<StaticArray<int, 3>, 3> matrix3;
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
//--------------- Zero Matrix (CTCI 1.8) ---------------------//
//------------------------------------------------------------//
//O(rows * cols)
template <typename T, int SizeR, int SizeC>
static inline void ZeroMatrix(StaticArray<StaticArray<T, SizeC>, SizeR>& inMatrix_){
	Array<size_t> rows;
	Array<size_t> cols;

	for(size_t i = 0; i < SizeR; i++){
		for(size_t j = 0; j < SizeC; j++){
			if(inMatrix_[i][j] == 0){
				rows.Add(j);
				cols.Add(i);
			}
		}
	}

	for(size_t i = 0; i < rows.Size(); i++){
		for(size_t j = 0; j < SizeR; j++){
			inMatrix_[j][rows[i]] = 0;
		}
	}

	for(size_t i = 0; i < cols.Size(); i++){
		for(size_t j = 0; j < SizeC; j++){
			inMatrix_[cols[i]][j] = 0;
		}
	}
}

TEST_CASE("StaticArray Zero Matrix", "[static_array_zero_matrix]"){
	StaticArray<StaticArray<int, 4>, 3> matrix;
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
	for(size_t i = 0; i < NumPeople; i++){
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

	StaticArray<int, 3> heights1;
	heights1[0] = 180;
	heights1[1] = 165;
	heights1[2] = 170;

	REQUIRE(names1.GetSize() == heights1.GetSize());
	StaticArray<String, 3> sorted1 = SortPeopleByHeight(names1, heights1);
	REQUIRE(sorted1.GetSize() == names1.GetSize());
	REQUIRE(sorted1[0] == "John");
	REQUIRE(sorted1[1] == "Emma");
	REQUIRE(sorted1[2] == "Mary");

	//-----------------------------------------//

	StaticArray<String, 3> names2;
	names2[0] = "Alice";
	names2[1] = "Bob";
	names2[2] = "Bob";

	StaticArray<int, 3> heights2;
	heights2[0] = 155;
	heights2[1] = 185;
	heights2[2] = 150;

	REQUIRE(names2.GetSize() == heights2.GetSize());
	StaticArray<String, 3> sorted2 = SortPeopleByHeight(names2, heights2);
	REQUIRE(sorted2.GetSize() == names2.GetSize());
	REQUIRE(sorted2[0] == "Bob");
	REQUIRE(sorted2[1] == "Alice");
	REQUIRE(sorted2[2] == "Bob");
}