#ifndef GADGET_ANIMATION_ANIM_CLIP_H
#define GADGET_ANIMATION_ANIM_CLIP_H

#include "Animation/Loaders/AssimpAnimLoader.h"
#include "Data/DList.h"
#include "Data/HashTable.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "Resource/Resource.h"

namespace Gadget{
	template <class T>
	struct KeyFrame{
		float time;
		T value;

		constexpr KeyFrame(float time_, const T& value_) : time(Math::Clamp(0.0f, 1.0f, time_)), value(value_){}
	};

	using VectorKey = KeyFrame<Vector3>;
	using QuatKey = KeyFrame<Quaternion>;

	class AnimClip : public Resource{
	public:
		AnimClip(float length_, const HashTable<StringID, DList<VectorKey>>& posKeys_, const HashTable<StringID, DList<QuatKey>>& rotKeys_, const HashTable<StringID, DList<VectorKey>>& scaleKeys_) : length(length_), posKeys(posKeys_), rotKeys(rotKeys_), scaleKeys(scaleKeys_){}

		virtual ~AnimClip() override{}

		static constexpr const char* typeName = "AnimClip";

	private:
		const float length;
		const HashTable<StringID, DList<VectorKey>> posKeys;
		const HashTable<StringID, DList<QuatKey>> rotKeys;
		const HashTable<StringID, DList<VectorKey>> scaleKeys;
	};

	class AnimClipResourceContainer : public ResourceContainer{
	public:
		AnimClipResourceContainer(const std::string& path_) : ResourceContainer(AnimClip::typeName, path_){}

		virtual Resource* LoadResource() override{
			return AssimpAnimLoader::LoadAnimClip(path, 0);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(AnimClip);
		}
	};
}

#endif //!GADGET_ANIMATION_ANIM_CLIP_H