#include "ObjLoader.h"

#include <algorithm>
#include <sstream>

#include "Graphics/Mesh.h"
#include "Core/FileSystem.h"
#include "Math/Math.h"

using namespace Gadget;

Mesh* ObjLoader::LoadMesh(const std::string& filePath_){
	GADGET_ASSERT(FileSystem::FileExists(filePath_), "Tried to load non-existent file [" + filePath_ + "]!");
	if(!FileSystem::FileExists(filePath_)){
		Debug::Log(SID("OBJLOADER"), "Tried to load non-existent file [" + filePath_ + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	std::vector<Vector3> tempPos = std::vector<Vector3>();
	std::vector<Vector3> tempNorm = std::vector<Vector3>();
	std::vector<Vector2> tempTex = std::vector<Vector2>();
	std::vector<VertIndex> tempIndices = std::vector<VertIndex>();

	const auto fileData = FileSystem::ReadFile(filePath_); //TODO - Reading files like this is synchronous (reads the whole file, then processes it), would be better to read and process asynchronously

	//Reasonable estimates for how much space we'll need
	tempPos.reserve(fileData.size() / 4);
	tempNorm.reserve(fileData.size() / 4);
	tempTex.reserve(fileData.size() / 4);
	tempIndices.reserve((fileData.size() * 3) / 4);

	for(const std::string& s : fileData){
		if(s.empty()){
			continue;
		}

		std::string firstWord = s.substr(0, s.find(" "));
		std::string remainder = s.substr(s.find(" "), s.size());

		if(firstWord == "v"){
			//Vertex Position
			tempPos.push_back(ProcessVector3(remainder));
		}else if(firstWord == "vt"){
			//Texture Coordinate
			tempTex.push_back(ProcessVector2(remainder));
		}else if(firstWord == "vn"){
			//Normal
			tempNorm.push_back(ProcessVector3(remainder));
		}else if(firstWord == "f"){
			//Faces
			std::replace(remainder.begin(), remainder.end(), '/', ' '); //Remove slashes to allow the stringstream to work later
			ProcessIndices(remainder, tempIndices);
		}
	}

	if(tempPos.empty()){
		Debug::Log(SID("OBJLOADER"), "An error occurred trying to load OBJ file [" + filePath_ + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	std::vector<VertIndex> uniqueVerts;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	
	uniqueVerts.reserve(tempIndices.size());
	vertices.reserve(tempIndices.size());
	indices.reserve(tempIndices.size());

	for(int i = 0; i < tempIndices.size(); i++){
		int uniqueIndex = -1;
		for(int j = 0; j < uniqueVerts.size(); j++){
			if(tempIndices[i] == uniqueVerts[j]){
				uniqueIndex = j;
				break;
			}
		}

		if(uniqueIndex < 0){
			// < 0 means this is a unique vertex, add it to the list
			uniqueVerts.push_back(tempIndices[i]);
			vertices.push_back(Vertex(tempPos[tempIndices[i].pos], tempNorm[tempIndices[i].norm], tempTex[tempIndices[i].tex]));
			indices.push_back(static_cast<uint32_t>(vertices.size() - 1));
		}else{
			//The vertex is already in the list, simply use the index of that vertex
			indices.push_back(uniqueIndex);
		}
	}

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	return new Mesh(vertices, indices);
}

Vector2 ObjLoader::ProcessVector2(const std::string& data_){
	Vector2 v = Vector2::Zero();
	std::stringstream data(data_);

	data >> v.x;
	data >> v.y;

	//Not strictly necessary but sometimes we get some very tiny garbage from the stringstream
	if(Math::IsNearZero(v.x)){
		v.x = 0.0f;
	}

	if(Math::IsNearZero(v.y)){
		v.y = 0.0f;
	}

	return v;
}

Vector3 ObjLoader::ProcessVector3(const std::string& data_){
	Vector3 v = Vector3::Zero();
	std::stringstream data(data_);

	data >> v.x;
	data >> v.y;
	data >> v.z;

	//Not strictly necessary but sometimes we get some very tiny garbage from the stringstream
	if(Math::IsNearZero(v.x)){
		v.x = 0.0f;
	}

	if(Math::IsNearZero(v.y)){
		v.y = 0.0f;
	}

	if(Math::IsNearZero(v.z)){
		v.z = 0.0f;
	}

	return v;
}

void ObjLoader::ProcessIndices(const std::string& data_, std::vector<VertIndex>& indexList_){
	std::stringstream data(data_);
	uint32_t tmp1 = 0;
	uint32_t tmp2 = 0;
	uint32_t tmp3 = 0;

	data >> tmp1;
	data >> tmp2;
	data >> tmp3;
	indexList_.push_back(VertIndex(tmp1 - 1, tmp2 - 1, tmp3 - 1));

	data >> tmp1;
	data >> tmp2;
	data >> tmp3;
	indexList_.push_back(VertIndex(tmp1 - 1, tmp2 - 1, tmp3 - 1));

	data >> tmp1;
	data >> tmp2;
	data >> tmp3;
	indexList_.push_back(VertIndex(tmp1 - 1, tmp2 - 1, tmp3 - 1));
}