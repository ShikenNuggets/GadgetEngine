#version 460 core

out vec4 color;

in vec2 outTexCoord;

uniform sampler2D screenTexture;

void main(){
	color = texture(screenTexture, outTexCoord);
}