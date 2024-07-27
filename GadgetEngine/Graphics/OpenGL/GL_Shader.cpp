#include "GL_Shader.h"

#include "App.h"
#include "Debug.h"
#include "Core/FileSystem.h"

using namespace Gadget;

GL_Shader::GL_Shader(const std::string& vertPath_, const std::string& fragPath_) : Shader(), shader(0){
	GADGET_BASIC_ASSERT(!vertPath_.empty());
	GADGET_BASIC_ASSERT(!fragPath_.empty());
	GADGET_BASIC_ASSERT(FileSystem::FileExists(vertPath_));
	GADGET_BASIC_ASSERT(FileSystem::FileExists(fragPath_));
	GADGET_ASSERT(App::GetCurrentRenderAPI() == Renderer::API::OpenGL, "Tried to execute OpenGL commands on non-OpenGL render API!");

	std::string vertCodeStr = FileSystem::ReadFileToString(vertPath_);
	GADGET_BASIC_ASSERT(!vertCodeStr.empty());
	if(vertCodeStr.empty()){
		Debug::ThrowFatalError(SID("RENDER"), "Could not load vertex shader code from [" + vertPath_ + "]!", ErrorCode::FileIO, __FILE__, __LINE__);
	}

	std::string fragCodeStr = FileSystem::ReadFileToString(fragPath_);
	GADGET_BASIC_ASSERT(!fragCodeStr.empty());
	if(fragCodeStr.empty()){
		Debug::ThrowFatalError(SID("RENDER"), "Could not load fragment shader code from [" + fragPath_ + "]!", ErrorCode::FileIO, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GADGET_BASIC_ASSERT(vertShader != 0);
	if(vertShader == 0){
		Debug::ThrowFatalError(SID("RENDER"), "Can't create a new vertex shader!", ErrorCode::OpenGL_Error, __FILE__, __LINE__);
	}

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GADGET_BASIC_ASSERT(fragShader != 0);
	if(fragShader == 0){
		Debug::ThrowFatalError(SID("RENDER"), "Can't create a new shader!", ErrorCode::OpenGL_Error, __FILE__, __LINE__);
	}

	const char* vertCode = vertCodeStr.c_str();
	const char* fragCode = fragCodeStr.c_str();
	glShaderSource(vertShader, 1, &vertCode, nullptr);
	glShaderSource(fragShader, 1, &fragCode, nullptr);

	GLint status = 0;

	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	GADGET_BASIC_ASSERT(status != GL_FALSE);
	if(status == GL_FALSE){
		Debug::ThrowFatalError(SID("RENDER"), "Could not compile Vertex Shader! GL Error: " + GetShaderLog(vertShader), ErrorCode::OpenGL_ShaderCompileFailed, __FILE__, __LINE__);
	}

	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	GADGET_BASIC_ASSERT(status != GL_FALSE);
	if(status == GL_FALSE){
		Debug::ThrowFatalError(SID("RENDER"), "Could not compile Fragment Shader! GL Error: " + GetShaderLog(fragShader), ErrorCode::OpenGL_ShaderCompileFailed, __FILE__, __LINE__);
	}

	shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	GADGET_BASIC_ASSERT(status != GL_FALSE);
	if(status == GL_FALSE){
		Debug::ThrowFatalError(SID("RENDER"), "Could not link Shader! GL Error: " + GetShaderLog(shader), ErrorCode::OpenGL_Error, __FILE__, __LINE__);
	}

	glDetachShader(shader, fragShader);
	glDetachShader(shader, vertShader);
	glDeleteShader(fragShader);
	glDeleteShader(vertShader);

	GADGET_BASIC_ASSERT(shader != 0);
}

GL_Shader::~GL_Shader(){
	GADGET_BASIC_ASSERT(shader != 0);
	glDeleteProgram(shader);
}

GLuint GL_Shader::GetShaderProgram(){
	return shader;
}

void GL_Shader::Bind(){
	GADGET_BASIC_ASSERT(shader != 0);
	glUseProgram(shader);
}

void GL_Shader::Unbind(){
	glUseProgram(0);
}

void GL_Shader::BindInt(StringID uniformName_, int value_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniform1iv(uniforms[uniformName_], 1, &value_);
	}
}

void GL_Shader::BindFloat(StringID uniformName_, float value_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniform1fv(uniforms[uniformName_], 1, &value_);
	}
}

void GL_Shader::BindVector2(StringID uniformName_, const Vector2& vec_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniform2fv(uniforms[uniformName_], 1, vec_);
	}
}

void GL_Shader::BindVector3(StringID uniformName_, const Vector3& vec_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniform3fv(uniforms[uniformName_], 1, vec_);
	}
}

void GL_Shader::BindVector4(StringID uniformName_, const Vector4& vec_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniform4fv(uniforms[uniformName_], 1, vec_);
	}
}

void GL_Shader::BindMatrix3(StringID uniformName_, const Matrix3& mat3_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniformMatrix3fv(uniforms[uniformName_], 1, GL_FALSE, mat3_);
	}
}

void GL_Shader::BindMatrix4(StringID uniformName_, const Matrix4& mat4_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniformMatrix4fv(uniforms[uniformName_], 1, GL_FALSE, mat4_);
	}
}

void GL_Shader::BindColor(StringID uniformName_, const Color& color_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniform4fv(uniforms[uniformName_], 1, color_);
	}
}

std::string GL_Shader::GetShaderLog(GLuint shader_){
	std::string errorLog = "";

	GLsizei errorLogSize = 0;
	glGetProgramiv(shader_, GL_INFO_LOG_LENGTH, &errorLogSize);

	if(errorLogSize > 0){
		errorLog.resize(errorLogSize);
		glGetProgramInfoLog(shader_, errorLogSize, &errorLogSize, &errorLog[0]);
	}

	return errorLog;
}