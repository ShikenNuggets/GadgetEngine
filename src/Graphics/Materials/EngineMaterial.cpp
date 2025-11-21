#include "EngineMaterial.h"

#include "App.h"

using namespace Gadget;

EngineMaterial::EngineMaterial(const Color& color_, bool materialInfoDeferred_) : colors(), materialInfo(nullptr){
	colors.push_back(color_);

	if(!materialInfoDeferred_){
		CreateMaterialInfo();
	}
}

EngineMaterial::EngineMaterial(const Color& colorA_, const Color& colorB_, const Color& colorC_, bool materialInfoDeferred_) : colors(), materialInfo(nullptr){
	colors.push_back(colorA_);
	colors.push_back(colorB_);
	colors.push_back(colorC_);

	if(!materialInfoDeferred_){
		CreateMaterialInfo();
	}
}

EngineMaterial::EngineMaterial(const std::vector<Color>& colors_, bool materialInfoDeferred_) : colors(colors_), materialInfo(nullptr){
	if(!materialInfoDeferred_){
		CreateMaterialInfo();
	}
}

EngineMaterial::~EngineMaterial(){
	delete materialInfo;
	materialInfo = nullptr;
}

void EngineMaterial::CreateMaterialInfo(){
	materialInfo = App::GetRenderer().GenerateAPIMaterialInfo(colors);
}