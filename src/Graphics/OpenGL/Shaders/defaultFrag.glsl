#version 460 core

struct PointLight{
	vec3 position;
	vec4 lightColor;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	vec4 lightColor;
	float constant;
	float linear;
	float quadratic;
};

struct DirLight{
	vec3 direction;
	vec4 lightColor;
};

out vec4 color;

in vec3 outNormal;
in vec2 outTexCoord;
in vec3 outFragPos;

uniform sampler2D aTexture;
uniform vec3 viewPos;

uniform int numPointLights;
uniform int numSpotLights;
uniform int numDirLights;

const int gMaxLights = 8;

uniform PointLight pointLights[gMaxLights];
uniform SpotLight spotLights[gMaxLights];
uniform DirLight dirLights[gMaxLights];

//Function Prototypes
vec3 GetPointLightShading(PointLight light, vec3 viewDir, vec3 matColor);
vec3 GetSpotLightShading(SpotLight light, vec3 viewDir, vec3 matColor);
vec3 GetDirLightShading(DirLight light, vec3 viewDir, vec3 matColor);

void main(){
	vec4 matColor = texture(aTexture, outTexCoord);
	vec3 viewDir = normalize(viewPos - outFragPos);

	vec3 result = vec3(0.0, 0.0, 0.0);
	for(int i = 0; i < numPointLights && i < gMaxLights; i++){
		result += GetPointLightShading(pointLights[i], viewDir, matColor.xyz);
	}

	for(int i = 0; i < numSpotLights && i < gMaxLights; i++){
		result += GetSpotLightShading(spotLights[i], viewDir, matColor.xyz);
	}

	for(int i = 0; i < numDirLights && i < gMaxLights; i++){
		result += GetDirLightShading(dirLights[i], viewDir, matColor.xyz);
	}

	float alpha = matColor.w;
	color = vec4(result, alpha);
}

vec3 GetPointLightShading(PointLight light, vec3 viewDir, vec3 matColor){
	float shininess = 32; //TODO - Material property
	float ambientValue = 0.1;

	vec3 lightDir = normalize(light.position - outFragPos);
	float diffuseValue = max(dot(outNormal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, outNormal);
	float specularValue = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	float distance = length(light.position - outFragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.lightColor.xyz * ambientValue * attenuation * matColor;
	vec3 diffuse = light.lightColor.xyz * diffuseValue * attenuation * matColor;
	vec3 specular = light.lightColor.xyz * specularValue * attenuation * matColor;
	return ambient + diffuse + specular;
}

vec3 GetSpotLightShading(SpotLight light, vec3 viewDir, vec3 matColor){
	float shininess = 32; //TODO - Material property
	float ambientValue = 0.1;

	vec3 lightDir = normalize(light.position - outFragPos);
	float diffuseValue = max(dot(outNormal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, outNormal);
	float specularValue = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	float distance = length(light.position - outFragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.lightColor.xyz * ambientValue * attenuation * intensity * matColor;
	vec3 diffuse = light.lightColor.xyz * diffuseValue * attenuation * intensity * matColor;
	vec3 specular = light.lightColor.xyz * specularValue * attenuation * intensity * matColor;
	return ambient + diffuse + specular;
}

vec3 GetDirLightShading(DirLight light, vec3 viewDir, vec3 matColor){
	float shininess = 32; //TODO - Material property
	float ambientValue = 0.1;

	vec3 lightDir = normalize(-light.direction);
	float diffuseValue = max(dot(outNormal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, outNormal);
	float specularValue = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = light.lightColor.xyz * ambientValue * matColor;
	vec3 diffuse = light.lightColor.xyz * diffuseValue * matColor;
	vec3 specular = light.lightColor.xyz * specularValue * matColor;
	return ambient + diffuse + specular;
}