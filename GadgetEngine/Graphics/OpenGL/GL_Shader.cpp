#include "GL_Shader.h"

#include "Debug.h"
#include "Core/FileSystem.h"

using namespace Gadget;

GL_Shader::GL_Shader(const std::string& vertPath_, const std::string& fragPath_) : Shader(), shader(0){
	std::string vertCodeStr = FileSystem::ReadFileToString(vertPath_);
	if(vertCodeStr.empty()){
		Debug::Log("Could not load vertex shader code from [" + vertPath_ + "]!", Debug::FatalError, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	std::string fragCodeStr = FileSystem::ReadFileToString(fragPath_);
	if(fragCodeStr.empty()){
		Debug::Log("Could not load fragment shader code from [" + fragPath_ + "]!", Debug::FatalError, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	if(vertShader == 0 || fragShader == 0){
		Debug::Log("Can't create a new shader!", Debug::FatalError, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	const char* vertCode = vertCodeStr.c_str();
	const char* fragCode = fragCodeStr.c_str();
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
	return shader;
}

void GL_Shader::Bind(){
	glUseProgram(shader);
}

void GL_Shader::Unbind(){
	glUseProgram(0);
}

void GL_Shader::BindMatrix4(StringID uniformName_, const Matrix4& mat4_){
	GLuint uniformID = glGetUniformLocation(GetShaderProgram(), uniformName_.GetString().c_str()); //TODO - This function is fairly expensive, cache the value from it
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, mat4_);
}

std::string GL_Shader::GetShaderLog(GLuint shader_){
	std::string errorLog;

	GLsizei errorLogSize = 0;
	glGetProgramiv(shader_, GL_INFO_LOG_LENGTH, &errorLogSize);
	errorLog.resize(errorLogSize);
	glGetProgramInfoLog(shader_, errorLogSize, &errorLogSize, &errorLog[0]);

	return errorLog;
}