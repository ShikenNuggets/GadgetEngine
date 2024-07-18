#include "AssimpModelLoader.h"

//Hide warnings from external code that we can't/won't modify
#pragma warning(disable : 26451)	//Possible arithmetic overflow
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#pragma warning(default : 26451)

#include "Core/FileSystem.h"
#include "Graphics/Mesh.h"
#include "Math/Math.h"
#include "Math/Vector.h"

using namespace Gadget;

Mesh* AssimpModelLoader::LoadMesh(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());
	GADGET_BASIC_ASSERT(FileSystem::FileExists(filePath_));

	std::vector<Submesh> submeshes;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath_, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
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
		indices.reserve(mesh->mNumFaces * 3);

		for(unsigned int j = 0; j < mesh->mNumVertices; j++){
			if(mesh->mNormals == nullptr){
				verts.push_back(Vertex(
					Vector3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z),
					Vector3::Forward(),
					Vector2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
				));
			}else{
				verts.push_back(Vertex(
					Vector3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z),
					Vector3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z),
					Vector2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
				));
			}
		}

		for(unsigned int j = 0; j < mesh->mNumFaces; j++){
			for(unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++){
				indices.push_back(mesh->mFaces[j].mIndices[k]);
			}
		}

		submeshes_.push_back(Submesh(verts, indices));
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++){
		ProcessNode(node->mChildren[i], scene, submeshes_);
	}
}