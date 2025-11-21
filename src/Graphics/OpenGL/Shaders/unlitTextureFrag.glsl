#version 460 core

out vec4 color;

in vec3 outNormal;
in vec2 outTexCoord;
in vec3 outFragPos;

uniform sampler2D aTexture;

void main(){
	color = texture(aTexture, outTexCoord);
}