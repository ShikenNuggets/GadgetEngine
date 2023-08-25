#include "Font.h"

#include <delaunator.hpp>

using namespace Gadget;

std::vector<Vector2> Font::CalculatePolygonForGlyph(unsigned char glyph_){
	GADGET_BASIC_ASSERT(glyph_ < glyphs.size());
	GlyphData& glyphData = glyphs[glyph_];

	std::vector<Vector2> points;
    std::vector<double> coords;

    coords.push_back(glyphData.points[0].xCoordinate);
    coords.push_back(glyphData.points[0].yCoordinate);
    points.push_back(Vector2(glyphData.points[0].xCoordinate, glyphData.points[0].yCoordinate));
    
	for(size_t i = 1; i < glyphData.points.size(); i++){
        coords.push_back(glyphData.points[i].xCoordinate + points.back().x);
        coords.push_back(glyphData.points[i].yCoordinate + points.back().y);

        points.push_back(Vector2(glyphData.points[i].xCoordinate + points.back().x, glyphData.points[i].yCoordinate + points.back().y));
	}

    delaunator::Delaunator d(coords); //TODO - Remove this depency, ideally this algorithm could be more tightly integrated into GadgetEngine's code and this specific usecase

    std::vector<Vector2> returnVal;
    for(size_t i = 0; i < d.triangles.size(); i += 3){
        returnVal.push_back(Vector2(static_cast<float>(d.coords[2 * d.triangles[i]]),       static_cast<float>(d.coords[2 * d.triangles[i] + 1])));
        returnVal.push_back(Vector2(static_cast<float>(d.coords[2 * d.triangles[i + 1]]),   static_cast<float>(d.coords[2 * d.triangles[i + 1] + 1])));
        returnVal.push_back(Vector2(static_cast<float>(d.coords[2 * d.triangles[i + 2]]),   static_cast<float>(d.coords[2 * d.triangles[i + 2] + 1])));
    }

    return returnVal;
}

std::vector<std::vector<Vector2>> Font::CalculatePolygonsForString(const std::string& text_){
    std::vector<std::vector<Vector2>> returnValue;
    for(unsigned char c : text_){
        returnValue.push_back(CalculatePolygonForGlyph(c));
    }

    return returnValue;
}