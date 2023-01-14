#ifndef GADGET_RANDOM_H
#define GADGET_RANDOM_H

#include <chrono>

namespace Gadget{
	//TODO - Implement custom RNG instead of using rand (Mersenne Twister? Xorshift? KISS99?)
	class Random{
	public:
		static void SetSeed(){
			srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		}
		
		template <class T>
		static T Range(T min_, T max_){
			return rand() % max_ + min_;
		}

		template<>
		float Range(float min_, float max_){
			return min_ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_ - min_)));
		}
	};
}

#endif //!GADGET_RANDOM_H