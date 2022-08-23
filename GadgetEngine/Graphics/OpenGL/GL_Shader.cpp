#include "GL_Shader.h"

#include "Debug.h"

using namespace Gadget;

//TODO - Obviously load these from files instead
constexpr char* vertCode = 
"#version 460 core \n \
layout(location = 0) in vec3 vertPos; \n \
void main(){ \n \
	gl_Position.xyz = vertPos; \n \
	gl_Position.w = 1.0; \n \
}";

constexpr char* fragCode = 
"#version 460 core \n \
out vec3 color; \n \
void main(){ \n \
	color = vec3(1.0f, 0.0f, 0.0f); \n \
}";

GL_Shader::GL_Shader() : Shader(), shader(0){
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	if(vertShader == 0 || fragShader == 0){
		Debug::Log("Can't create a new shader!", Debug::FatalError, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	glShaderSource(vertShader, 1, &vertCode, nullptr);
	glShaderSource(fragShader, 1, &fragCode, nullptr);

	GLint status = 0;

	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		Debug::Log("Could not compile Vertex Shader! GL Error: " + GetShaderLog(vertShader), Debug::FatalError, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		Debug::Log("Could not compile Fragment Shader! GL Error: " + GetShaderLog(fragShader), Debug::FatalError, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if(status == GL_FALSE){
		Debug::Log("Could not link Shader! GL Error: " + GetShaderLog(shader), Debug::FatalError, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	glDetachShader(shader, fragShader);
	glDetachShader(shader, vertShader);
	glDeleteShader(fragShader);
	glDeleteShader(vertShader);
}

GL_Shader::~GL_Shader(){
	glDeleteProgram(shader);
}

GLuint GL_Shader::GetShaderProgram(){
	return GLuint();
}

void GL_Shader::Bind(){
	glUseProgram(shader);
}

void GL_Shader::Unbind(){
	glUseProgram(0);
}

std::string GL_Shader::GetShaderLog(GLuint shader_){
	std::string errorLog;

	GLsizei errorLogSize = 0;
	glGetProgramiv(shader_, GL_INFO_LOG_LENGTH, &errorLogSize);
	errorLog.resize(errorLogSize);
	glGetProgramInfoLog(shader_, errorLogSize, &errorLogSize, &errorLog[0]);

	return errorLog;
}