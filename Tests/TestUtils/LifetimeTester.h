#ifndef GADGET_TESTS_TEST_UTILS_LIFETIME_TESTER_H
#define GADGET_TESTS_TEST_UTILS_LIFETIME_TESTER_H

#include <atomic>
#include <string>

class LifetimeTester{
public:
	LifetimeTester(int a_, int b_, const std::string& c_) : a(a_), b(b_), c(c_){ numConstructs++; }
	~LifetimeTester(){ numDestructs++; }

	LifetimeTester(const LifetimeTester& other_) : a(other_.a), b(other_.b), c(other_.c){ numCopies++; }
	LifetimeTester(LifetimeTester&& other_) noexcept : a(other_.a), b(other_.b), c(std::move(other_.c)){ numMoves++; }

	LifetimeTester& operator=(const LifetimeTester& other_){
		a = other_.a;
		b = other_.b;
		c = other_.c;

		numCopies++;
		return *this;
	};

	LifetimeTester& operator=(LifetimeTester&& other_) noexcept{
		a = other_.a;
		b = other_.b;
		c = std::move(other_.c);

		numMoves++;
		return *this;
	};

	int GetA() const{ return a; }
	int GetB() const{ return b; }
	const std::string& GetC() const{ return c; }

	static void Reset(){
		numConstructs = 0;
		numDestructs = 0;
		numCopies = 0;
		numMoves = 0;
	}

	static inline volatile std::atomic<int> numConstructs;
	static inline volatile std::atomic<int> numDestructs;
	static inline volatile std::atomic<int> numCopies;
	static inline volatile std::atomic<int> numMoves;

private:
	int a;
	int b;
	std::string c;
};

#endif //!GADGET_TESTS_TEST_UTILS_LIFETIME_TESTER_H