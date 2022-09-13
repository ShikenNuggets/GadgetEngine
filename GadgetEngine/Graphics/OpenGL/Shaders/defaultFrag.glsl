#version 460 core

out vec4 color;

in vec3 outNormal;
in vec2 outTexCoord;
in vec3 outFragPos;

uniform sampler2D aTexture;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec4 lightColor;

void main(){
	//Ambient
	vec3 ambient = 0.1 * lightColor.xyz;

	//Diffuse
	vec3 norm = normalize(outNormal);
	vec3 lightDir = normalize(lightPos - outFragPos);
	float diffuseValue = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseValue * lightColor.xyz;

	//Specular
	vec3 viewDir = normalize(viewPos - outFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5 * spec * lightColor.xyz;

	color = vec4(ambient + diffuse + specular, 1.0) * texture(aTexture, outTexCoord);
}