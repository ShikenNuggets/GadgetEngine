#ifndef GADGET_TEXT_MESH_H
#define GADGET_TEXT_MESH_H

#include "FreetypeFont.h"
#include "Graphics/MeshInfo.h"
#include "Graphics/Shader.h"

namespace Gadget{
	class TextMesh{
	public:
		TextMesh(StringID font_, StringID shader_, const std::string& initialText_);
		~TextMesh();

		FreetypeFont* GetFont() const{ return font; }
		Shader* GetShader() const{ return shader; }
		std::string GetText() const{ return text; }
		MeshInfo* GetMeshInfo() const{ return meshInfo; }

		size_t GetTotalWidthInPixels() const;
		size_t GetTotalHeightInPixels() const;

		void SetText(const std::string& text_){ text = text_; }

	private:
		std::string text;
		StringID fontName;
		FreetypeFont* font;
		MeshInfo* meshInfo;
		StringID shaderName;
		Shader* shader;
	};
}

#endif //!GADGET_TEXT_MESH_H