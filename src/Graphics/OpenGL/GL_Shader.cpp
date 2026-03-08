#include "Graphics/OpenGL/GL_Shader.h"

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

	const std::string vertCodeStr = FileSystem::ReadFileToString(vertPath_);
	GADGET_BASIC_ASSERT(!vertCodeStr.empty());
	if(vertCodeStr.empty()){
		Debug::ThrowFatalError(SID("RENDER"), "Could not load vertex shader code from [" + vertPath_ + "]!", ErrorCode::FileIO, __FILE__, __LINE__);
	}

	const std::string fragCodeStr = FileSystem::ReadFileToString(fragPath_);
	GADGET_BASIC_ASSERT(!fragCodeStr.empty());
	if(fragCodeStr.empty()){
		Debug::ThrowFatalError(SID("RENDER"), "Could not load fragment shader code from [" + fragPath_ + "]!", ErrorCode::FileIO, __FILE__, __LINE__);
		//TODO - Handle fatal error
	}

	const GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GADGET_BASIC_ASSERT(vertShader != 0);
	if(vertShader == 0){
		Debug::ThrowFatalError(SID("RENDER"), "Can't create a new vertex shader!", ErrorCode::ThirdParty_Error, __FILE__, __LINE__);
	}

	const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GADGET_BASIC_ASSERT(fragShader != 0);
	if(fragShader == 0){
		Debug::ThrowFatalError(SID("RENDER"), "Can't create a new shader!", ErrorCode::ThirdParty_Error, __FILE__, __LINE__);
	}

	const char* vertCode = vertCodeStr.c_str();
	const char* fragCode = fragCodeStr.c_str();
	glShaderSource(vertShader, 1, &vertCode, nullptr);
	glShaderSource(fragShader, 1, &fragCode, nullptr);

	GLint status = 0;

	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	GADGET_ASSERT(status != GL_FALSE, "Vertex shader compile failed! GL Error: {}", GetShaderLog(vertShader));
	if(status == GL_FALSE){
		Debug::ThrowFatalError(SID("RENDER"), "Could not compile Vertex Shader! GL Error: " + GetShaderLog(vertShader), ErrorCode::ThirdParty_Error, __FILE__, __LINE__);
	}

	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	GADGET_ASSERT(status != GL_FALSE, "Fragment shader compile failed! GL Error: {}", GetShaderLog(fragShader));
	if(status == GL_FALSE){
		Debug::ThrowFatalError(SID("RENDER"), "Could not compile Fragment Shader! GL Error: " + GetShaderLog(fragShader), ErrorCode::ThirdParty_Error, __FILE__, __LINE__);
	}

	shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	GADGET_ASSERT(status != GL_FALSE, "Shader linking failed! GL Error: {}", GetProgramLog(shader));
	if(status == GL_FALSE){
		Debug::ThrowFatalError(SID("RENDER"), "Could not link Shader! GL Error: " + GetProgramLog(shader), ErrorCode::ThirdParty_Error, __FILE__, __LINE__);
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

GLuint GL_Shader::GetShaderProgram() const{
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

		std::array<float, 2> data = { static_cast<float>(vec_.x), static_cast<float>(vec_.y) };
		glUniform2fv(uniforms[uniformName_], 1, data.data());
	}
}

void GL_Shader::BindVector3(StringID uniformName_, const Vector3& vec_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);

		std::array<float, 3> data = { static_cast<float>(vec_.x), static_cast<float>(vec_.y), static_cast<float>(vec_.z) };
		glUniform3fv(uniforms[uniformName_], 1, data.data());
	}
}

void GL_Shader::BindVector4(StringID uniformName_, const Vector4& vec_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);

		std::array<float, 4> data = { static_cast<float>(vec_.x), static_cast<float>(vec_.y), static_cast<float>(vec_.z), static_cast<float>(vec_.w) };
		glUniform4fv(uniforms[uniformName_], 1, data.data());
	}
}

void GL_Shader::BindMatrix3(StringID uniformName_, const Matrix3& mat3_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);

		std::array<float, 9> data = { static_cast<float>(mat3_[0]), static_cast<float>(mat3_[1]), static_cast<float>(mat3_[2]), static_cast<float>(mat3_[3]), static_cast<float>(mat3_[4]), static_cast<float>(mat3_[5]), static_cast<float>(mat3_[6]), static_cast<float>(mat3_[7]), static_cast<float>(mat3_[8]) };
		glUniformMatrix3fv(uniforms[uniformName_], 1, GL_FALSE, data.data());
	}
}

void GL_Shader::BindMatrix4(StringID uniformName_, const Matrix4& mat4_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);

		std::array<float, 16> data = { static_cast<float>(mat4_[0]), static_cast<float>(mat4_[1]), static_cast<float>(mat4_[2]), static_cast<float>(mat4_[3]), static_cast<float>(mat4_[4]), static_cast<float>(mat4_[5]), static_cast<float>(mat4_[6]), static_cast<float>(mat4_[7]), static_cast<float>(mat4_[8]), static_cast<float>(mat4_[9]), static_cast<float>(mat4_[10]), static_cast<float>(mat4_[11]), static_cast<float>(mat4_[12]), static_cast<float>(mat4_[13]), static_cast<float>(mat4_[14]), static_cast<float>(mat4_[15]) };
		glUniformMatrix4fv(uniforms[uniformName_], 1, GL_FALSE, data.data());
	}
}

void GL_Shader::BindColor(StringID uniformName_, const Color& color_){
	GADGET_BASIC_ASSERT(uniformName_ != StringID::None);

	const auto colorArray = color_.AsArray();

	if(HasUniform(uniformName_)){
		GADGET_BASIC_ASSERT(uniforms[uniformName_] >= 0);
		glUniform4fv(uniforms[uniformName_], 1, colorArray.data());
	}
}

std::string GL_Shader::GetShaderLog(GLuint shader_){
	std::string errorLog = "";

	GLsizei errorLogSize = 0;
	glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &errorLogSize);

	if(errorLogSize > 0){
		errorLog.resize(errorLogSize);
		glGetShaderInfoLog(shader_, errorLogSize, &errorLogSize, errorLog.data());
	}

	return errorLog;
}

std::string GL_Shader::GetProgramLog(GLuint program_){
	std::string errorLog = "";

	GLsizei errorLogSize = 0;
	glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &errorLogSize);

	if(errorLogSize > 0){
		errorLog.resize(errorLogSize);
		glGetProgramInfoLog(program_, errorLogSize, &errorLogSize, errorLog.data());
	}

	return errorLog;
}