#ifndef GADGET_ASSIMP_MODEL_LOADER_H
#define GADGET_ASSIMP_MODEL_LOADER_H

#include <string>

#include <assimp/scene.h>

namespace Gadget{
	//Forward Declaration
	class Mesh;

	class AssimpModelLoader{
	public:
		static Mesh* LoadMesh(const std::string& filePath_);

	private:
		static Mesh* ProcessNode(const aiNode* node, const aiScene* scene);
	};
}

#endif //!GADGET_ASSIMP_MODEL_LOADER_H