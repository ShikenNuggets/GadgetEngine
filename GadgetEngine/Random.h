#ifndef GADGET_RANDOM_H
#define GADGET_RANDOM_H

#include <chrono>

#include "Graphics/Color.h"

namespace Gadget{
	//TODO - Implement custom RNG instead of using rand (Mersenne Twister? Xorshift? KISS99?)
	namespace Random{
		inline void SetSeed(){
			srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		}
		
		template <class T>
		inline T Range(T min_, T max_){
			return rand() % max_ + min_;
		}

		template<>
		inline float Range(float min_, float max_){
			return min_ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_ - min_)));
		}

		inline Color RandomColor(bool randomAlpha_ = false){
			float alpha = 1.0f;
			if(randomAlpha_){
				alpha = Range(0.0f, 1.0f);
			}

			return Color(Range(0.0f, 1.0f), Range(0.0f, 1.0f), Range(0.0f, 1.0f), alpha);
		}
	};
}

#endif //!GADGET_RANDOM_H