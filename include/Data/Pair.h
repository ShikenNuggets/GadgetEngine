#ifndef GADGET_DATA_PAIR_H
#define GADGET_DATA_PAIR_H

namespace Gadget{
	template <typename T, typename U>
	struct Pair{
	public:
		Pair(const T& first_, const U& second_) : first(first_), second(second_){}

		T first;
		U second;
	};
}

#endif //!GADGET_DATA_PAIR_H