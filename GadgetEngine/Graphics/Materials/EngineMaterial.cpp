#include "EngineMaterial.h"

using namespace Gadget;

EngineMaterial::EngineMaterial(const Color& color_) : colors(), materialInfo(nullptr){
	colors.push_back(color_);
}

EngineMaterial::EngineMaterial(const Color& colorA_, const Color& colorB_, const Color& colorC_) : colors(), materialInfo(nullptr){
	colors.push_back(colorA_);
	colors.push_back(colorB_);
	colors.push_back(colorC_);
}

EngineMaterial::EngineMaterial(const std::vector<Color>& colors_) : colors(colors_), materialInfo(nullptr){}