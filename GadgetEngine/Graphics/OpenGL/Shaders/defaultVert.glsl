#version 460 core

layout(location = 0) in vec3 vertPos;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(){
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vertPos, 1.0f);
}