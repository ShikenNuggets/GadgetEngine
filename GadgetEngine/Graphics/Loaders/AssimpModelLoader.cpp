#include "AssimpModelLoader.h"

//Hide warnings from external code that we can't/won't modify
#pragma warning(disable : 26451)	//Possible arithmetic overflow
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#pragma warning(default : 26451)

#include "Animation/AnimMesh.h"
#include "Animation/Skeleton.h"
#include "Core/FileSystem.h"
#include "Graphics/Mesh.h"
#include "Math/Math.h"
#include "Math/Vector.h"

using namespace Gadget;

//TODO - aiProcess_RemoveComponent? to removed unused data (e.g. vertex colors)
//TODO - aiProcess_GenSmoothNormals?
static constexpr unsigned int gBaseFlags = 
	aiProcess_Triangulate
	| aiProcess_FlipUVs
	| aiProcess_JoinIdenticalVertices
	| aiProcess_LimitBoneWeights
	| aiProcess_ImproveCacheLocality
	| aiProcess_RemoveRedundantMaterials
	| aiProcess_FixInfacingNormals
	| aiProcess_FindInvalidData
	| aiProcess_GenUVCoords
	| aiProcess_OptimizeMeshes
	| aiProcess_OptimizeGraph
	| aiProcess_PopulateArmatureData;

#ifdef GADGET_DEBUG
	static constexpr unsigned int gLoadFlags = gBaseFlags | aiProcess_ValidateDataStructure;
#else
	static constexpr unsigned int gLoadFlags = gBaseFlags;
#endif // GADGET_DEBUG

Mesh* AssimpModelLoader::LoadMesh(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());
	GADGET_BASIC_ASSERT(FileSystem::FileExists(filePath_));

	std::vector<Submesh> submeshes;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath_, gLoadFlags);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		Debug::Log("AssImp could not load model! AssImp Error: " + std::string(importer.GetErrorString()), Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	ProcessNode(scene->mRootNode, scene, submeshes);
	return new Mesh(submeshes);
}

void AssimpModelLoader::ProcessNode(const aiNode* node, const aiScene* scene, std::vector<Submesh>& submeshes_){
	GADGET_BASIC_ASSERT(node != nullptr);
	GADGET_BASIC_ASSERT(scene != nullptr);

	submeshes_.reserve(submeshes_.size() + node->mNumMeshes);

	for(size_t i = 0; i < node->mNumMeshes; i++){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		std::vector<Vertex> verts;
		std::vector<unsigned int> indices;

		verts.reserve(mesh->mNumVertices);
		indices.reserve(static_cast<size_t>(mesh->mNumFaces) * 3);

		for(unsigned int j = 0; j < mesh->mNumVertices; j++){
			if(mesh->mNormals == nullptr){
				verts.emplace_back(
					Vector3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z),
					Vector3::Forward(),
					Vector2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
				);
			}else{
				verts.emplace_back(
					Vector3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z),
					Vector3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z),
					Vector2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
				);
			}
		}

		for(unsigned int j = 0; j < mesh->mNumFaces; j++){
			for(unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++){
				indices.push_back(mesh->mFaces[j].mIndices[k]);
			}
		}

		submeshes_.emplace_back(verts, indices);
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++){
		ProcessNode(node->mChildren[i], scene, submeshes_);
	}
}

AnimMesh* AssimpModelLoader::LoadAnimMesh(const std::string& filePath_){
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* scene = importer.ReadFile(filePath_, gLoadFlags);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		GADGET_LOG_ERROR(SID("ASSET"), "AssImp could not load model! AssImp Error: " + std::string(importer.GetErrorString()));
		return nullptr;
	}

	Array<AnimSubmesh> subMeshes;
	Array<Joint> joints;
	subMeshes.Reserve(scene->mNumMeshes);

	ProcessAnimNode(scene->mRootNode, scene, subMeshes, joints);

	Skeleton sk = Skeleton(ConvertMatrix4(scene->mRootNode->mTransformation).Inverse());
	for(const auto& j : joints){
		sk.AddJoint(j);
	}

	GADGET_BASIC_ASSERT(sk.IsValidSkeleton());

	return new AnimMesh(subMeshes, sk);
}

void AssimpModelLoader::ProcessAnimNode(const aiNode* node_, const aiScene* scene_, Array<AnimSubmesh>& inSubMeshes_, Array<Joint>& inJoints_){
	GADGET_BASIC_ASSERT(node_ != nullptr);
	GADGET_BASIC_ASSERT(scene_ != nullptr);

	for(size_t i = 0; i < node_->mNumMeshes; i++){
		aiMesh* mesh = scene_->mMeshes[node_->mMeshes[i]];

		Array<AnimVertex> verts;
		Array<unsigned int> indices;

		verts.Reserve(mesh->mNumVertices);
		indices.Reserve(static_cast<size_t>(mesh->mNumFaces) * 3);

		//Vertices
		for(unsigned int j = 0; j < mesh->mNumVertices; j++){
			if(mesh->mNormals == nullptr){
				verts.Add(AnimVertex(
					Vector3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z),
					Vector3::Forward(),
					Vector2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
				));
			}else{
				verts.Add(AnimVertex(
					Vector3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z),
					Vector3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z),
					Vector2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
				));
			}
		}

		//Indices
		for(unsigned int j = 0; j < mesh->mNumFaces; j++){
			for(unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++){
				indices.Add(mesh->mFaces[j].mIndices[k]);
			}
		}

		//Bones
		for(unsigned int j = 0; j < mesh->mNumBones; j++){
			aiBone* bone = mesh->mBones[j];
			GADGET_BASIC_ASSERT(bone != nullptr);

			Joint joint;
			joint.name = StringID::ProcessString(bone->mName.C_Str());
			joint.inverseBindPose = ConvertMatrix4(bone->mOffsetMatrix);

			if(!inJoints_.Contains(joint)){
				if(bone->mNode == nullptr || bone->mNode->mParent == nullptr){
					joint.parentID = -1;
				}else{
					joint.parentID = GetJointIndex(inJoints_, StringID::ProcessString(bone->mNode->mParent->mName.C_Str()));
				}

				inJoints_.Add(joint);
			}

			//Per-vertex skinning data
			for(unsigned int k = 0; k < bone->mNumWeights; k++){
				const aiVertexWeight& weight = bone->mWeights[k];
				GADGET_BASIC_ASSERT(verts.Size() > weight.mVertexId);
				verts[weight.mVertexId].AddWeight(static_cast<int32_t>(inJoints_.Size()) - 1, weight.mWeight);
			}
		}

		inSubMeshes_.Add(AnimSubmesh(verts, indices));
	}

	for(unsigned int i = 0; i < node_->mNumChildren; i++){
		ProcessAnimNode(node_->mChildren[i], scene_, inSubMeshes_, inJoints_);
	}
}

int32_t AssimpModelLoader::GetJointIndex(Array<Joint>& joints, StringID name_){
	for(int32_t i = 0; i < joints.Size(); i++){
		if(joints[i].name == name_){
			return i;
		}
	}

	return -1;
}

Matrix4 AssimpModelLoader::ConvertMatrix4(const aiMatrix4x4& m_){
	Matrix4 m;
	m[0] = m_.a1;	m[4] = m_.a2;	m[8] = m_.a3;	m[12] = m_.a4;
	m[1] = m_.b1;	m[5] = m_.b2;	m[9] = m_.b3;	m[13] = m_.b4;
	m[2] = m_.c1;	m[6] = m_.c2;	m[10] = m_.c3;	m[14] = m_.c4;
	m[3] = m_.d1;	m[7] = m_.d2;	m[11] = m_.d3;	m[15] = m_.d4;
	return m;
}