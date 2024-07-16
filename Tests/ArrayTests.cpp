#include <Gadget.h>
#include <Data/Array.h>

#include "Catch2/catch_amalgamated.hpp"

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

	const size_t n = inMatrix_.Size();
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