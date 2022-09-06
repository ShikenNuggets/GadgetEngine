#include "ObjLoader.h"

#include <algorithm>
#include <sstream>

#include "Core/FileSystem.h"
#include "Math/Math.h"

using namespace Gadget;

Mesh* ObjLoader::LoadMesh(const std::string& filePath_){
	_ASSERT(FileSystem::FileExists(filePath_)); //Tried to load a non-existent file!
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
			//Vertex
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

	//TODO - FIX THIS!!!
	//This logic is for converting the index lists that the OBJ gives us into something graphics APIs/the engine can process
	//For unclear reasons, this current version has the wrong winding order for *some* triangles
	//For now we're just doing indices the most inefficient brain-dead way possible, but obviously we need this to work correctly long-term
	std::vector<VertIndex> uniqueVerts;
	uniqueVerts.reserve(tempIndices.size());
	uniqueVerts.push_back(tempIndices[0]);

	for(size_t i = 1; i < tempIndices.size(); i++){
		bool isUnique = true;

		//TODO - Bring back this for loop when everything's fixed
		/*for(const auto& v : uniqueVerts){
			if(tempIndices[i] == v){
				isUnique = false;
				break;
			}
		}*/

		if(isUnique){
			uniqueVerts.push_back(tempIndices[i]);
		}
	}

	std::vector<Vertex> vertices;
	vertices.reserve(uniqueVerts.size());
	for(const auto& v : uniqueVerts){
		vertices.push_back(Vertex(tempPos[v.pos], tempNorm[v.norm], tempTex[v.tex]));
	}

	std::vector<uint32_t> indices;
	indices.reserve(tempIndices.size());

	//TODO - Bring back this for loop when everything's fixed
	/*for(const auto& ti : tempIndices){
		for(int i = 0; i < uniqueVerts.size(); i++){
			if(ti == uniqueVerts[i]){
				indices.push_back(i);
				break;
			}
		}
	}*/

	//TODO - Remove this for loop when everything's fixed
	for(int i = 0; i < vertices.size(); i++){
		indices.push_back(i);
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