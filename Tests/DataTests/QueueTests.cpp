#include <Gadget.h>
#include <Data/Queue.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//---------------- Animal Shelter (CTCI 3.6) -----------------//
//------------------------------------------------------------//

class Animal{
public:
	enum class Type{
		Dog,
		Cat
	};

	constexpr Animal(Type type_, int64_t arrivalTime_) : type(type_), arrivalTime(arrivalTime_){}

	constexpr Type GetType() const{ return type; }
	constexpr int64_t GetArrivalTime() const{ return arrivalTime; }

private:
	Type type;
	int64_t arrivalTime;
};

class AnimalShelterQueue{
public:
	AnimalShelterQueue() : currentTime(0), cats(), dogs(){}

	void AddDog(){
		dogs.Add(Animal(Animal::Type::Dog, currentTime));
		currentTime++;
	}

	void AddCat(){
		cats.Add(Animal(Animal::Type::Cat, currentTime));
		currentTime++;
	}

	Animal DequeueAny(){
		GADGET_BASIC_ASSERT(!cats.IsEmpty() || !dogs.IsEmpty());

		int64_t catTime = std::numeric_limits<int64_t>::max();
		int64_t dogTime = std::numeric_limits<int64_t>::max();

		if(!cats.IsEmpty()){
			catTime = cats.Peek().GetArrivalTime();
		}

		if(!dogs.IsEmpty()){
			dogTime = dogs.Peek().GetArrivalTime();
		}

		GADGET_BASIC_ASSERT(catTime != dogTime);
		if(catTime < dogTime){
			return cats.Remove();
		}else{
			return dogs.Remove();
		}
	}

	Animal DequeueCat(){
		GADGET_BASIC_ASSERT(!cats.IsEmpty());
		return cats.Remove();
	}

	Animal DequeueDog(){
		GADGET_BASIC_ASSERT(!dogs.IsEmpty());
		return dogs.Remove();
	}

	int64_t Size() const{ return cats.Size() + dogs.Size(); }
	int64_t NumCats() const{ return cats.Size(); }
	int64_t NumDogs() const{ return dogs.Size(); }
	bool IsEmpty() const{ return cats.IsEmpty() || dogs.IsEmpty(); }

private:
	int64_t currentTime;
	Queue<Animal> cats;
	Queue<Animal> dogs;
};

TEST_CASE("Queue Animal Shelter", "[queue_animal_shelter]"){
	AnimalShelterQueue queue;	REQUIRE(queue.IsEmpty());
	queue.AddDog();				REQUIRE(queue.Size() == 1);	REQUIRE(queue.NumCats() == 0);	REQUIRE(queue.NumDogs() == 1);
	queue.AddDog();				REQUIRE(queue.Size() == 2);	REQUIRE(queue.NumCats() == 0);	REQUIRE(queue.NumDogs() == 2);
	queue.AddCat();				REQUIRE(queue.Size() == 3);	REQUIRE(queue.NumCats() == 1);	REQUIRE(queue.NumDogs() == 2);
	queue.AddDog();				REQUIRE(queue.Size() == 4);	REQUIRE(queue.NumCats() == 1);	REQUIRE(queue.NumDogs() == 3);
	queue.AddCat();				REQUIRE(queue.Size() == 5);	REQUIRE(queue.NumCats() == 2);	REQUIRE(queue.NumDogs() == 3);
	queue.AddCat();				REQUIRE(queue.Size() == 6);	REQUIRE(queue.NumCats() == 3);	REQUIRE(queue.NumDogs() == 3);
	queue.AddDog();				REQUIRE(queue.Size() == 7);	REQUIRE(queue.NumCats() == 3);	REQUIRE(queue.NumDogs() == 4);

	Animal animal = queue.DequeueDog();
	REQUIRE(queue.Size() == 6);
	REQUIRE(animal.GetArrivalTime() == 0);
	REQUIRE(animal.GetType() == Animal::Type::Dog);

	animal = queue.DequeueAny();
	REQUIRE(queue.Size() == 5);
	REQUIRE(animal.GetArrivalTime() == 1);
	REQUIRE(animal.GetType() == Animal::Type::Dog);

	animal = queue.DequeueCat();
	REQUIRE(queue.Size() == 4);
	REQUIRE(animal.GetArrivalTime() == 2);
	REQUIRE(animal.GetType() == Animal::Type::Cat);

	animal = queue.DequeueCat();
	REQUIRE(queue.Size() == 3);
	REQUIRE(animal.GetArrivalTime() == 4);
	REQUIRE(animal.GetType() == Animal::Type::Cat);
}