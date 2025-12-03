#include "Graphics/OpenGL/GL_MeshInfo.h"

#include "App.h"
#include "Graphics/Vertex.h"

using namespace Gadget;

struct TempFloatVertex
{
	TVec3<float> position;
	TVec3<float> normal;
	TVec2<float> texCoords;
};

GL_MeshInfo::GL_MeshInfo(const Submesh& mesh_) : MeshInfo(mesh_.indices.size()), vao(0), vbo(0), ebo(0){
	GADGET_ASSERT(App::GetCurrentRenderAPI() == Renderer::API::OpenGL, "Tried to execute OpenGL commands on non-OpenGL render API!");

	std::vector<TempFloatVertex> gpuVerts;
	gpuVerts.reserve(mesh_.vertices.size());
	for (const auto& vert : mesh_.vertices)
	{
		TempFloatVertex tempVert;
		tempVert.position = TVec3<float>(vert.position.x, vert.position.y, vert.position.z);
		tempVert.normal = TVec3<float>(vert.normal.x, vert.normal.y, vert.normal.z);
		tempVert.texCoords = TVec2<float>(vert.texCoords.x, vert.texCoords.y);
		gpuVerts.push_back(tempVert);
	}

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);

	glNamedBufferData(vbo, static_cast<GLsizeiptr>(gpuVerts.size() * sizeof(TempFloatVertex)), gpuVerts.data(), GL_STATIC_DRAW);
	glNamedBufferData(ebo, static_cast<GLsizeiptr>(mesh_.indices.size() * sizeof(uint32_t)), mesh_.indices.data(), GL_STATIC_DRAW);

	//Position
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(TempFloatVertex, position));

	//Normal
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(TempFloatVertex, normal));

	//Texture Coordinates
	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayAttribBinding(vao, 2, 0);
	glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(TempFloatVertex, texCoords));

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(TempFloatVertex));
	glVertexArrayElementBuffer(vao, ebo);

	GADGET_BASIC_ASSERT(vao != 0);
	GADGET_BASIC_ASSERT(vbo != 0);
	GADGET_BASIC_ASSERT(ebo != 0);
}

GL_MeshInfo::~GL_MeshInfo(){
	GADGET_BASIC_ASSERT(vao != 0);
	GADGET_BASIC_ASSERT(vbo != 0);
	GADGET_BASIC_ASSERT(ebo != 0);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void GL_MeshInfo::Bind(){
	GADGET_BASIC_ASSERT(vao != 0);

	glBindVertexArray(vao);
}

void GL_MeshInfo::Unbind(){
	glBindVertexArray(0);
}

//-------------------------------------------------------------------------------------------------//
//--------------------------------- GL_AnimMeshInfo -----------------------------------------------//
//-------------------------------------------------------------------------------------------------//

struct TempFloatAnimVertex
{
	TVec3<float> position;
	TVec3<float> normal;
	TVec2<float> texCoords;
	StaticArray<int32_t, AnimVertex::maxJointWeights> jointIDs;
	StaticArray<float, AnimVertex::maxJointWeights> jointWeights;
};

GL_AnimMeshInfo::GL_AnimMeshInfo(const AnimSubmesh& mesh_) : MeshInfo(mesh_.indices.Size()), vao(0), vbo(0), ebo(0){
	GADGET_ASSERT(App::GetCurrentRenderAPI() == Renderer::API::OpenGL, "Tried to execute OpenGL commands on non-OpenGL render API!");

	std::vector<TempFloatAnimVertex> gpuVerts;
	gpuVerts.reserve(mesh_.vertices.Size());
	for(const auto& vert : mesh_.vertices)
	{
		TempFloatAnimVertex tempVert;
		tempVert.position = TVec3<float>(vert.vertex.position.x, vert.vertex.position.y, vert.vertex.position.z);
		tempVert.normal = TVec3<float>(vert.vertex.normal.x, vert.vertex.normal.y, vert.vertex.normal.z);
		tempVert.texCoords = TVec2<float>(vert.vertex.texCoords.x, vert.vertex.texCoords.y);
		tempVert.jointIDs = vert.jointIDs;
		tempVert.jointWeights = vert.jointWeights;
		gpuVerts.push_back(tempVert);
	}

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);

	glNamedBufferData(vbo, static_cast<GLsizeiptr>(gpuVerts.size() * sizeof(TempFloatAnimVertex)), gpuVerts.data(), GL_STATIC_DRAW);
	glNamedBufferData(ebo, static_cast<GLsizeiptr>(mesh_.indices.Size() * sizeof(uint32_t)), &mesh_.indices[0], GL_STATIC_DRAW);

	//Position
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(TempFloatAnimVertex, position));

	//Normal
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(TempFloatAnimVertex, normal));

	//Texture Coordinates
	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayAttribBinding(vao, 2, 0);
	glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(TempFloatAnimVertex, texCoords));

	//Joint IDs
	glEnableVertexArrayAttrib(vao, 3);
	glVertexArrayAttribBinding(vao, 3, 0);
	glVertexArrayAttribFormat(vao, 3, AnimVertex::maxJointWeights, GL_FLOAT, GL_FALSE, offsetof(TempFloatAnimVertex, jointIDs)); //TODO - This should technically be GL_INT, but for some reason that doesn't behave correctly

	//Joint Weights
	glEnableVertexArrayAttrib(vao, 4);
	glVertexArrayAttribBinding(vao, 4, 0);
	glVertexArrayAttribFormat(vao, 4, AnimVertex::maxJointWeights, GL_FLOAT, GL_FALSE, offsetof(TempFloatAnimVertex, jointWeights));

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(TempFloatAnimVertex));
	glVertexArrayElementBuffer(vao, ebo);

	GADGET_BASIC_ASSERT(vao != 0);
	GADGET_BASIC_ASSERT(vbo != 0);
	GADGET_BASIC_ASSERT(ebo != 0);
}

GL_AnimMeshInfo::~GL_AnimMeshInfo(){
	GADGET_BASIC_ASSERT(vao != 0);
	GADGET_BASIC_ASSERT(vbo != 0);
	GADGET_BASIC_ASSERT(ebo != 0);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void GL_AnimMeshInfo::Bind(){
	GADGET_BASIC_ASSERT(vao != 0);

	glBindVertexArray(vao);
}

void GL_AnimMeshInfo::Unbind(){
	glBindVertexArray(0);
}