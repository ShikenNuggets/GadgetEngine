#version 460 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNorm;
layout(location = 2) in vec2 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 texCoord;

void main(){
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vertPos, 1.0f);
	texCoord = texCoords;
}