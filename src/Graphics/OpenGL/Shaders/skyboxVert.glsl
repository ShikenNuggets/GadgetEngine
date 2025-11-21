#version 460 core

layout(location = 0) in vec3 vertPos;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 outTexCoord;

void main(){
	outTexCoord = normalize(vertPos);
	gl_Position = ((projectionMatrix * viewMatrix) * vec4(vertPos, 1.0f)).xyww;
}