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

		constexpr KeyFrame(float time_, const T& value_) : time(time_), value(value_){}
	};

	using VectorKey = KeyFrame<Vector3>;
	using QuatKey = KeyFrame<Quaternion>;

	using VectorNode = DList<VectorKey>::Node;
	using QuatNode = DList<QuatKey>::Node;

	template <class T1, class T2>
	struct ClipSearchResult{
		explicit ClipSearchResult(const T2* node_ = nullptr, float time_ = 0.0f, bool useInterpolation_ = true) : result(), node(node_){
			if(node_ != nullptr && (node_->next == nullptr || !useInterpolation_)){
				result = node_->value.value;
			}else if(useInterpolation_ && node_ != nullptr && node_->next != nullptr){
				GADGET_ASSERT(node_->prev == nullptr || node_->value.time <= time_, "Node passed to ClipSearchResult has an unexpectedly high time value");
				GADGET_ASSERT(node_->next->value.time >= time_, "Node->next passed to ClipSearchResult has an unexpectedly low time value");
				const float delta = node_->next->value.time - node_->value.time;
				const float factor = Math::Clamp(0.0f, 1.0f, (time_ - node_->value.time) / delta);
				result = T1::Lerp(node_->value.value, node_->next->value.value, factor);
			}
		}

		T1 result;
		const T2* node;
	};

	using VectorResult = ClipSearchResult<Vector3, VectorNode>;
	using QuatResult = ClipSearchResult<Quaternion, QuatNode>;

	struct FullClipSearchResult{
		FullClipSearchResult(const VectorResult& pos_, const QuatResult& rot_, const VectorResult& scale_) : posNode(pos_.node), rotNode(rot_.node), scaleNode(scale_.node){
			result = Matrix4::Identity();
			result *= Matrix4::Translate(pos_.result);
			result *= rot_.result.ToMatrix4();
			result *= Matrix4::Scale(scale_.result);
		}

		Matrix4 result;
		const VectorNode* posNode;
		const QuatNode* rotNode;
		const VectorNode* scaleNode;
	};
}

#endif //!GADGET_ANIMATION_KEY_FRAME_H