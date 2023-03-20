#version 460 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNorm;
layout(location = 2) in vec2 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform int charPos;

void main(){
	vec3 outFragPos = vec3(modelMatrix * vec4(vertPos.x, vertPos.y, 0.0, 1.0));
	gl_Position = (projectionMatrix * viewMatrix) * vec4(outFragPos, 1.0f);

	//gl_Position = vec4((vertPos.x / 10) + pos.x + charPos * 0.1, (vertPos.y / 10) + pos.y, 0.0, 1.0);
}