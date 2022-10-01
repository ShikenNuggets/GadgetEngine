#version 460 core

out vec4 color;

in vec2 outTexCoord;

uniform sampler2D screenTexture;

void main(){
	color = vec4(vec3(1.0 - texture(screenTexture, outTexCoord)), 1.0);
}