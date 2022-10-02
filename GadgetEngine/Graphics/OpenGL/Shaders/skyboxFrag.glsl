#version 460 core

out vec4 color;

in vec3 outTexCoord;

uniform samplerCube skybox;

void main(){
	color = texture(skybox, outTexCoord);
}