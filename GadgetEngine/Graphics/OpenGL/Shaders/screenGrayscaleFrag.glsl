#version 460 core

out vec4 color;

in vec2 outTexCoord;

uniform sampler2D screenTexture;

void main(){
	color = texture(screenTexture, outTexCoord);
	float avg = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722); //Weighted average looks more physically correct than true average
	color = vec4(avg, avg, avg, 1.0);
}