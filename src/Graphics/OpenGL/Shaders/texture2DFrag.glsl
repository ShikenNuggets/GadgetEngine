#version 460 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D tex;
uniform vec4 tintColor;

void main(){
    color = tintColor * texture(tex, TexCoords);
}