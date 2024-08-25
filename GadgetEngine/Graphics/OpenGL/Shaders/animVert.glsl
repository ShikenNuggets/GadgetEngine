#version 460 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNorm;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4 boneWeights;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 outNormal;
out vec2 outTexCoord;
out vec3 outFragPos;

uniform mat4 bones[128];

void main(){
	mat4 boneTransform = bones[boneIDs[0]] * boneWeights[0];
	boneTransform += bones[boneIDs[1]] * boneWeights[1];
	boneTransform += bones[boneIDs[2]] * boneWeights[2];
	boneTransform += bones[boneIDs[3]] * boneWeights[3];

	outNormal = normalize(normalMatrix * vertPos);
	outTexCoord = texCoords;
	outFragPos = vec3((modelMatrix * (boneTransform * vec4(vertPos, 1.0))));

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * (boneTransform * vec4(vertPos, 1.0));
}