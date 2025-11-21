#version 460 core

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec2 texCoords;

out vec2 outTexCoord;

void main(){
	gl_Position = vec4(vertPos.x, vertPos.y, 0.0, 1.0);
	outTexCoord = texCoords;
}