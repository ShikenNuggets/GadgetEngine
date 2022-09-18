#version 460 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNorm;
layout(location = 2) in vec2 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 outNormal;
out vec2 outTexCoord;
out vec3 outFragPos;

void main(){
	outNormal = normalize(normalMatrix * vertPos);
	outTexCoord = texCoords;
	outFragPos = vec3(modelMatrix * vec4(vertPos, 1.0));

	gl_Position = (projectionMatrix * viewMatrix) * vec4(outFragPos, 1.0f);
}