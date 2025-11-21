#ifndef GADGET_ASSIMP_MODEL_LOADER_H
#define GADGET_ASSIMP_MODEL_LOADER_H

#include <string>
#include <vector>

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

#include "Data/Array.h"
#include "Math/Matrix.h"

namespace Gadget{
	//Forward Declaration
	struct Submesh;
	class Mesh;
	struct AnimSubmesh;
	class AnimMesh;
	struct Joint;

	class AssimpModelLoader{
	public:
		static Mesh* LoadMesh(const std::string& filePath_);
		static AnimMesh* LoadAnimMesh(const std::string& filePath_);

	private:
		static void ProcessNode(const aiNode* node, const aiScene* scene, std::vector<Submesh>& submeshes_);
		static void ProcessAnimNode(const aiNode* node_, const aiScene* scene_, Array<AnimSubmesh>& inSubMeshes_, Array<Joint>& inJoints_);

		static int32_t GetJointIndex(Array<Joint>& joints, StringID name_);
		static Matrix4 ConvertMatrix4(const aiMatrix4x4& m_);
	};
}

#endif //!GADGET_ASSIMP_MODEL_LOADER_H