#ifndef GADGET_TEXT_MESH_H
#define GADGET_TEXT_MESH_H

#include "Font.h"
#include "Graphics/MeshInfo.h"

namespace Gadget{
	class TextMesh{
	public:
		TextMesh(StringID font_, const std::string& initialText_);
		~TextMesh();

		size_t GetNumMeshes() const{ return meshInfos.size(); }
		MeshInfo* GetMeshInfo(size_t index_) const{ return meshInfos[index_]; }

	private:
		StringID fontName;
		Font* font;
		std::vector<MeshInfo*> meshInfos;
	};
}

#endif //!GADGET_TEXT_MESH_H