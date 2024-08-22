#ifndef GADGET_ANIMATION_KEY_FRAME_H
#define GADGET_ANIMATION_KEY_FRAME_H

#include "Data/DList.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

namespace Gadget{
	template <class T>
	struct KeyFrame{
		float time;
		T value;

		constexpr KeyFrame(float time_, const T& value_) : time(Math::Clamp(0.0f, 1.0f, time_)), value(value_){}
	};

	using VectorKey = KeyFrame<Vector3>;
	using QuatKey = KeyFrame<Quaternion>;

	using VectorNode = DList<VectorKey>::Node;
	using QuatNode = DList<QuatKey>::Node;

	template <class T1, class T2>
	struct ClipSearchResult{
		explicit ClipSearchResult(T2* node_ = nullptr) : result(), node(node_){
			if(node_ != nullptr){
				result = node_->value;
			}
		}

		T1 result;
		T2* node;
	};

	using VectorResult = ClipSearchResult<VectorKey, VectorNode>;
	using QuatResult = ClipSearchResult<QuatKey, QuatNode>;

	struct FullClipSearchResult{
		FullClipSearchResult(const VectorResult& pos_, const QuatResult& rot_, const VectorResult& scale_) : posNode(pos_.node), rotNode(rot_.node), scaleNode(scale_.node){
			result = Matrix4::Identity();
			result *= Matrix4::Translate(pos_.result.value);
			result *= rot_.result.value.ToMatrix4();
			result *= Matrix4::Scale(scale_.result.value);
		}

		Matrix4 result;
		VectorNode* posNode;
		QuatNode* rotNode;
		VectorNode* scaleNode;
	};
}

#endif //!GADGET_ANIMATION_KEY_FRAME_H