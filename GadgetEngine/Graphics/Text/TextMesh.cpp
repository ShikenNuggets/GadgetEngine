#include "TextMesh.h"

#include "App.h"

using namespace Gadget;

TextMesh::TextMesh(StringID font_, const std::string& initialText_) : fontName(font_), font(nullptr), meshInfos(){
	font = App::GetResourceManager().LoadResource<Font>(fontName);
	GADGET_BASIC_ASSERT(font != nullptr);

	auto glyphVerts = font->CalculatePolygonsForString(initialText_);

	for(const auto glyph : glyphVerts){
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		for(uint32_t i = 0; i < glyph.size(); i++){
			vertices.push_back(Vertex(Vector3(glyph[i].x, glyph[i].y, 1.0f), Vector3::Zero(), Vector2::Zero()));
			indices.push_back(i);
		}

		Mesh mesh = Mesh(vertices, indices);
		meshInfos.push_back(App::GetRenderer().GenerateAPIMeshInfo(mesh));
	}
}

TextMesh::~TextMesh(){
	App::GetResourceManager().UnloadResource(fontName);
}