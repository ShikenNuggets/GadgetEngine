#include "AssimpAnimLoader.h"

//Hide warnings from external code that we can't/won't modify
#pragma warning(disable : 4244)		//Possible loss of data from type conversion
#pragma warning(disable : 26451)	//Possible arithmetic overflow
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#pragma warning(default : 4244)
#pragma warning(default : 26451)

#include "Debug.h"
#include "Animation/AnimClip.h"
#include "Utils/StringID.h"

using namespace Gadget;

static constexpr unsigned int gBaseFlags = 0
	| aiProcess_ImproveCacheLocality
	| aiProcess_FindInvalidData
	| aiProcess_OptimizeGraph;

#ifdef GADGET_DEBUG
	static constexpr unsigned int gLoadFlags = gBaseFlags | aiProcess_ValidateDataStructure;
#else
	static constexpr unsigned int gLoadFlags = gBaseFlags;
#endif // GADGET_DEBUG

unsigned int AssimpAnimLoader::GetNumAnimClipsInFile(const std::string& filePath_){
	GADGET_ASSERT_FILE_EXISTS(filePath_);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath_, 0); //No flags, no need to do any special post-processing since we're literally just checking the number of animations
	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr){
		GADGET_LOG_ERROR(SID("ASSET"), "AssImp could not load data! AssImp Error: " + std::string(importer.GetErrorString()));
		return 0;
	}

	return scene->mNumAnimations;
}

AnimClip* AssimpAnimLoader::LoadAnimClip(const std::string& filePath_, unsigned int clipID_){
	GADGET_ASSERT_FILE_EXISTS(filePath_);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath_, gLoadFlags);
	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr){
		GADGET_LOG_ERROR(SID("ASSET"), "AssImp could not load data! AssImp Error: " + std::string(importer.GetErrorString()));
		return nullptr;
	}

	if(scene->HasAnimations() == false){
		GADGET_LOG_ERROR(SID("ASSET"), "AssImp scene has no animations!");
		return nullptr;
	}

	if(scene->mNumAnimations <= clipID_){
		GADGET_LOG_ERROR(SID("ASSET"), "AssImp scene has less than " + std::to_string(clipID_) + " animations!");
		return nullptr;
	}

	const aiAnimation* anim = scene->mAnimations[clipID_];
	GADGET_BASIC_ASSERT(anim != nullptr);
	if(anim == nullptr){
		GADGET_LOG_ERROR(SID("ASSET"), "AssImp scene contains invalid data!");
		return nullptr;
	}

	const float duration = static_cast<float>(anim->mDuration / anim->mTicksPerSecond);
	HashTable<StringID, DList<VectorKey>> posKeys;
	HashTable<StringID, DList<QuatKey>> rotKeys;
	HashTable<StringID, DList<VectorKey>> scaleKeys;

	for(unsigned int i = 0; i < anim->mNumChannels; i++){
		const aiNodeAnim* channel = anim->mChannels[i];
		const StringID modelNameSID = StringID::ProcessString(channel->mNodeName.C_Str());

		GADGET_BASIC_ASSERT(channel != nullptr);
		if(channel == nullptr){
			GADGET_LOG_WARNING(SID("ASSET"), "Null animation channel detected in AssImp data!");
			continue;
		}

		//Positions
		if(channel->mNumPositionKeys > 0){
			posKeys.Add(modelNameSID, DList<VectorKey>());
		}

		for(unsigned int j = 0; j < channel->mNumPositionKeys; j++){
			const aiVectorKey pKey = channel->mPositionKeys[j];
			posKeys[modelNameSID].Add(VectorKey(static_cast<float>(pKey.mTime / anim->mTicksPerSecond), ConvertVector3(pKey.mValue)));
		}

		//Rotations
		if(channel->mNumRotationKeys > 0){
			rotKeys.Add(modelNameSID, DList<QuatKey>());
		}

		for(unsigned int j = 0; j < channel->mNumRotationKeys; j++){
			const aiQuatKey rKey = channel->mRotationKeys[j];
			rotKeys[modelNameSID].Add(QuatKey(static_cast<float>(rKey.mTime / anim->mTicksPerSecond), ConvertQuaternion(rKey.mValue)));
		}

		//Scales
		if(channel->mNumScalingKeys > 0){
			scaleKeys.Add(modelNameSID, DList<VectorKey>());
		}

		for(unsigned int j = 0; j < channel->mNumScalingKeys; j++){
			const aiVectorKey sKey = channel->mScalingKeys[j];
			scaleKeys[modelNameSID].Add(VectorKey(static_cast<float>(sKey.mTime / anim->mTicksPerSecond), ConvertVector3(sKey.mValue)));
		}
	}

	return new AnimClip(duration, posKeys, rotKeys, scaleKeys);
}