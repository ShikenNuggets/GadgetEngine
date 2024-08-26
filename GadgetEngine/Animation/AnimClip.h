#ifndef GADGET_ANIMATION_ANIM_CLIP_H
#define GADGET_ANIMATION_ANIM_CLIP_H

#include "Animation/KeyFrame.h"
#include "Animation/Loaders/AssimpAnimLoader.h"
#include "Data/DList.h"
#include "Data/HashTable.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "Resource/Resource.h"

namespace Gadget{
	class AnimClip : public Resource{
	public:
		AnimClip(float length_, const HashTable<StringID, DList<VectorKey>>& posKeys_, const HashTable<StringID, DList<QuatKey>>& rotKeys_, const HashTable<StringID, DList<VectorKey>>& scaleKeys_) : length(length_), posKeys(posKeys_), rotKeys(rotKeys_), scaleKeys(scaleKeys_){}

		virtual ~AnimClip() override{}

		static constexpr const char* typeName = "AnimClip";

		float GetLength() const{ return length; }
		bool HasKeysForJoint(StringID jointName_) const;
		FullClipSearchResult GetTransformAtTime(StringID jointName_, float time_, const VectorNode* posNode_, const QuatNode* rotNode_, const VectorNode* scaleNode_) const;

		VectorResult GetTranslationAtTime(StringID jointName_, float time_, const VectorNode* posNode_) const;
		QuatResult GetRotationAtTime(StringID jointName_, float time_, const QuatNode* rotNode_) const;
		VectorResult GetScaleAtTime(StringID jointName_, float time_, const VectorNode* scaleNode_) const;

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