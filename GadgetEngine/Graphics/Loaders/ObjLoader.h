#ifndef GADGET_OBJ_LOADER_H
#define GADGET_OBJ_LOADER_H

#include "Graphics/Mesh.h"

namespace Gadget{
	struct VertIndex{
		constexpr VertIndex(uint32_t pos_, uint32_t tex_, uint32_t norm_) : pos(pos_), norm(norm_), tex(tex_){}

		uint32_t pos;
		uint32_t norm;
		uint32_t tex;

		constexpr bool operator ==(const VertIndex& i_) const{ return i_.pos == pos && i_.norm == norm && i_.tex == tex; }
	};

	class ObjLoader{
	public:
		static Mesh* LoadMesh(const std::string& filePath_);

	private:
		static Vector2 ProcessVector2(const std::string& data_);
		static Vector3 ProcessVector3(const std::string& data_);
		static void ProcessIndices(const std::string& data_, std::vector<VertIndex>& indexList_);
	};
}

#endif //!GADGET_OBJ_LOADER_H