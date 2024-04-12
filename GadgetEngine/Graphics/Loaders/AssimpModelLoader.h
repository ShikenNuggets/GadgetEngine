#ifndef GADGET_ASSIMP_MODEL_LOADER_H
#define GADGET_ASSIMP_MODEL_LOADER_H

#include <string>

//--------------------------------------------------------------------
//Undefine some "standard" macros that tend to cause problems
#undef min
#undef max
//--------------------------------------------------------------------

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 4244)		//Possible loss of data from type conversion
#pragma warning(disable : 26451)	//Possible arithmetic overflow
#include <assimp/scene.h>
#pragma warning(default : 4244)
#pragma warning(default : 26451)	//Possible arithmetic overflow

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