#version 460 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D aTexture;

void main(){
	color = texture(aTexture, texCoord);
}