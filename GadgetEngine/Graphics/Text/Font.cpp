#include "Font.h"

using namespace Gadget;

// Check if a Vector2 is inside a triangle
bool PointInsideTriangle(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c) {
    // Compute barycentric coordinates of Vector2 p
    float u = ((b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x)) / ((b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y));
    float v = ((c.x - a.x) * (p.y - a.y) - (c.y - a.y) * (p.x - a.x)) / ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
    // Check if Vector2 p is inside the triangle
    return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}

// Perform ear clipping triangulation on a polygon
std::vector<Vector2> earClipping(const std::vector<Vector2>& polygon) {
    // Initialize the list of vertices and triangles
    std::vector<Vector2> vertices = polygon;
    std::vector<int> triangles;
    // Continue until we have only one triangle left
    while(vertices.size() > 2) {
        // Find an ear vertex (i.e. a vertex that forms a triangle with its two neighbors)
        auto it = vertices.begin();
        while(it != vertices.end()) {
            auto prev = (it == vertices.begin()) ? vertices.end() - 1 : it - 1;
            auto next = (it == vertices.end() - 1) ? vertices.begin() : it + 1;
            if(PointInsideTriangle(*it, *prev, *next, vertices[(next == vertices.end() - 1) ? 0 : next - vertices.begin() + 1])) {
                break;
            }
            ++it;
        }
        // If no ear vertex was found, the polygon is not simple
        if(it == vertices.end()) {
            return std::vector<Vector2>();
        }
        // Add the ear vertex to the triangle list and remove it from the vertex list
        int i = it - vertices.begin();
        triangles.push_back(i);
        // Remove the ear vertex and its adjacent edges
        vertices.erase(it);
    }
    // Add the final triangle
    triangles.push_back(0);
    triangles.push_back(1);
    triangles.push_back(2);
    // Return the list of triangles
    std::vector<Vector2> result;
    for(int i = 0; i < triangles.size(); i += 3) {
        result.push_back(vertices[triangles[i]]);
        result.push_back(vertices[triangles[i + 1]]);
        result.push_back(vertices[triangles[i + 2]]);
    }
    return result;
}

std::vector<Vector2> Font::CalculatePolygonForGlyph(unsigned char glyph_){
	GADGET_BASIC_ASSERT(glyph_ < glyphs.size());
	GlyphData& glyphData = glyphs[glyph_];

	std::vector<Vector2> points;
    points.push_back(Vector2(glyphData.points[0].xCoordinate, glyphData.points[0].yCoordinate));
	for(size_t i = 1; i < glyphData.points.size(); i++){
        points.push_back(Vector2(glyphData.points[i].xCoordinate + points.back().x, glyphData.points[i].yCoordinate + points.back().y));
	}

    return earClipping(points);
}

std::vector<std::vector<Vector2>> Font::CalculatePolygonsForString(const std::string& text_){
    std::vector<std::vector<Vector2>> returnValue;
    for(unsigned char c : text_){
        returnValue.push_back(CalculatePolygonForGlyph(c));
    }

    return returnValue;
}