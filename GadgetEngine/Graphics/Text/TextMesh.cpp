#include "TextMesh.h"

#include "App.h"

using namespace Gadget;

TextMesh::TextMesh(StringID font_, StringID shader_, const std::string& initialText_) : text(initialText_), fontName(font_), font(nullptr), meshInfo(nullptr), shaderName(shader_), shader(nullptr){
	font = App::GetResourceManager().LoadResource<FreetypeFont>(fontName);
	GADGET_BASIC_ASSERT(font != nullptr);

	meshInfo = App::GetRenderer().GenerateAPIDynamicMeshInfo(4, 6);
	shader = App::GetRenderer().GenerateAPIShader(shader_);
}

TextMesh::~TextMesh(){
	delete meshInfo;

	App::GetResourceManager().UnloadResource(shaderName);
	App::GetResourceManager().UnloadResource(fontName);
}

size_t TextMesh::GetTotalWidthInPixels() const{
	size_t totalWidthInPixels = 0;
	for(char c : text){
		FreetypeFontCharacter ch = font->GetCharacters().at(c);
		totalWidthInPixels += (static_cast<size_t>(ch.advanceX) >> 6);
	}

	return totalWidthInPixels;
}

size_t TextMesh::GetTotalHeightInPixels() const{
	size_t totalHeightInPixels = 0;
	for(char c : text){
		FreetypeFontCharacter ch = font->GetCharacters().at(c);

		if(ch.rows > totalHeightInPixels){
			totalHeightInPixels = static_cast<size_t>(ch.rows);
		}
	}

	return totalHeightInPixels;
}