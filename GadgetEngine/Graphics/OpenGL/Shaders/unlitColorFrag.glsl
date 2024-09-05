#version 460 core

struct Material{
	vec4 color;
};

out vec4 color;

in vec3 outNormal;
in vec2 outTexCoord;
in vec3 outFragPos;

uniform Material material;

void main(){
	color = material.color;
}