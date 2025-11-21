#ifndef GADGET_SERIALIZERS_CONTAINER_SERIALIZER_H
#define GADGET_SERIALIZERS_CONTAINER_SERIALIZER_H

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26495)	//Uninitialized member variable
#pragma warning(disable : 26800)	//Use of a moved object
#pragma warning(disable : 26819)	//Unnanotated fallthrough in switch statement
#include <nlohmann/json.hpp>
#pragma warning(default : 26495)
#pragma warning(default : 26800)
#pragma warning(default : 26819)

#include "Data/Array.h"

namespace Gadget{
	template <class T>
	inline void to_json(nlohmann::json& j_, const Array<T>& array_){
		for(int64_t i = 0; i < array_.Size(); i++){
			to_json(j_[i], array_[i]);
		}
	}

	template <class T>
	inline void from_json(const nlohmann::json& j_, Array<T>& array_){
		for(const auto& j : j_){
			T value{}; //This only works if T has a default constructor... TODO - is there any way around this?
			from_json(j, value);
			array_.Add(value);
		}
	}
}

#endif //!GADGET_SERIALIZERS_CONTAINER_SERIALIZER_H