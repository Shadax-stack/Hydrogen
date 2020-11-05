#include "Shader.h"
#include <fstream>
#include <ios>
#include <memory>
#include <filesystem>
#include <map>

#define Program Handle

namespace Hydrogen {

	inline static unsigned int CompileShader(const char* source, unsigned int type) {
		//std::cout << source << std::endl;
		unsigned int Shader = glCall(glCreateShader(type));
		glCall(glShaderSource(Shader, 1, &source, nullptr));
		glCall(glCompileShader(Shader));
		int CompileStatus;
		glCall(glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileStatus));
		//Since GL_FALSE is false and GL_TRUE is true, we can use CompileStatus like a regular old boolean now
		if (CompileStatus) {
			return Shader;
		} else {
			int ShaderLogLength;
			glCall(glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &ShaderLogLength));
			std::unique_ptr<char> ShaderCompileLog(new char[static_cast<unsigned long long>(ShaderLogLength) + 1ULL]);
			ShaderCompileLog.get()[ShaderLogLength] = '\0';
			glCall(glGetShaderInfoLog(Shader, ShaderLogLength, &ShaderLogLength, ShaderCompileLog.get()));
			ThrowException(ShaderCompileLog.get());
			return 0;
		}
	}

	HydrogenShader::HydrogenShader(void) {

	}

	void HydrogenShader::CreateIdentifier(void) {
		Program = glCall(glCreateProgram());
	}

	void HydrogenShader::DeleteIdentifier(void) {
		glCall(glDeleteProgram(Program));
	}

	HydrogenShader::~HydrogenShader(void) {

	}

	void HydrogenShader::CompileShaderFiles(const char* vs, const char* fs) {
		//TODO: Optimize this bit of code later
		auto ReadFile = [](const char* path) { //I have to fix return-by-refrence errors with this peice of code
			std::ifstream reader;
			std::ostringstream buffer;
			reader.open(path);
			if (!reader.is_open()) ThrowException("Unable to read the file");
			buffer << reader.rdbuf();
			reader.close();
			return buffer;
		};
		CompileShaderSource(ReadFile(vs).str().c_str(), ReadFile(fs).str().c_str());
	}

	void HydrogenShader::CompileShaderSource(const char* vs, const char* fs) {
		unsigned int VertexShader = CompileShader(vs, GL_VERTEX_SHADER);
		unsigned int FragmentShader = CompileShader(fs, GL_FRAGMENT_SHADER);
		glCall(glAttachShader(Program, VertexShader));
		glCall(glAttachShader(Program, FragmentShader));
		glCall(glLinkProgram(Program));
 		glCall(glValidateProgram(Program));
		glCall(glDetachShader(Program, VertexShader));
		glCall(glDetachShader(Program, FragmentShader));
		glCall(glDeleteShader(VertexShader));
		glCall(glDeleteShader(FragmentShader));
		//This link-check code is useless (for logging) on some GPUs/drivers
		//On my GTX 980 with 445.87 drivers the compile log would be empty
		int LinkStatus;
		glCall(glGetProgramiv(Program, GL_LINK_STATUS, &LinkStatus));
		if (!LinkStatus) {
			int ProgramLogLength;
			glCall(glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &ProgramLogLength));
			std::unique_ptr<char> ProgramLinkLog(new char[static_cast<unsigned long long>(ProgramLogLength) + 1ULL]);
			ProgramLinkLog.get()[ProgramLogLength] = '\0';
			glCall(glGetProgramInfoLog(Program, ProgramLogLength, &ProgramLogLength, ProgramLinkLog.get()));
			ThrowException(ProgramLinkLog.get());
		}
	}

	void HydrogenShader::UseShader(void)  {
		glCall(glUseProgram(Program));
	}

	void HydrogenShader::UploadCamera(const char* name, const HydrogenCamera& camera) const {
		UploadCamera(std::string(name), camera);
	}

	//Yes, ik it's undreadable, the implementation is not magic
	//All you need to know it that it uniforms a camera to the shader
	void HydrogenShader::UploadCamera(const std::string & name, const HydrogenCamera& camera) const {
		UploadMatrix4x4f(name + ".ViewMatrix", camera.CreateViewMatrix());
		UploadMatrix4x4f(name + ".ProjectionMatrix", camera.CreateIsometricMatrix());
		UploadVector3f(name + ".Position", camera.GetPosition());
	}

	void HydrogenShader::UploadTransform(const char* name, const HydrogenTransform& transform) const {
		UploadTransform(std::string(name), transform);
	}

	void HydrogenShader::UploadTransform(const std::string& name, const HydrogenTransform& transform) const {
		UploadMatrix2x2f(name + ".TextureMatrix", transform.CreateTextureMatrix());
		UploadMatrix3x3f(name + ".NormalMatrix",transform.CreateNormalMatrix());
		UploadMatrix4x4f(name + ".ModelMatrix", transform.CreateModelMatrix());
	}

	void HydrogenShader::UploadTexture(const char* name, int binding) const {
		UploadInteger(name, binding);
	}

	void HydrogenShader::UploadTexture(const std::string& name, int binding) const {
		UploadTexture(name.c_str(), binding);
	}

	void HydrogenShader::UploadDirectLight(const char* name, const HydrogenDirectLight& light) const {
		UploadDirectLight(std::string(name), light);
	}

	void HydrogenShader::UploadDirectLight(const std::string& name, const HydrogenDirectLight& light) const {
		UploadVector3f(name + ".Direction", light.Direction);
		UploadVector3f(name + ".Color", light.Color);
	}

	void HydrogenShader::UploadPointLight(const char* name, const HydrogenPointLight& light) const {
		UploadPointLight(std::string(name), light);
	}

	void HydrogenShader::UploadPointLight(const std::string& name, const HydrogenPointLight& light) const {
		UploadVector3f(name + ".Position", light.Position);
		UploadVector3f(name + ".Luminosity", light.Luminosity);
	}

	void HydrogenShader::UploadFloat(const char* name, const float val) const {
		glCall(glUniform1fv(GetUniformLocation(name), 1, &val));
	}

	void HydrogenShader::UploadFloat(const std::string& name, const float val) const {
		UploadFloat(name.c_str(), val);
	}

	void HydrogenShader::UploadVector3f(const char* name, const glm::vec3& vec) const {
		glCall(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec)));
	}

	void HydrogenShader::UploadVector3f(const std::string& name, const glm::vec3& val) const {
		UploadVector3f(name.c_str(), val);
	}

	int HydrogenShader::GetUniformLocation(const char* name) const {
		int location = glCall(glGetUniformLocation(Program, name));
		return location;
	}

	int HydrogenShader::GetUniformLocation(const std::string& name) const {
		return GetUniformLocation(name.c_str());
	}

	void HydrogenShader::UploadInteger(const char* name, const int val) const { //Maybe replace with glUniform1iv, idk about textures tho
		glCall(glUniform1i(GetUniformLocation(name), val));
	}

	void HydrogenShader::UploadInteger(const std::string& name, const int val) const {
		UploadInteger(name.c_str(), val);
	}

	void HydrogenShader::UploadMatrix4x4f(const char* name, const glm::mat4& val) const {
		glCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val)));
	}

	void HydrogenShader::UploadMatrix4x4f(const std::string& name, const glm::mat4& val) const {
		UploadMatrix4x4f(name.c_str(), val);
	}

	void HydrogenShader::UploadMatrix2x2f(const char* name, const glm::mat2& val) const {
		glCall(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val)));
	}

	void HydrogenShader::UploadMatrix2x2f(const std::string& name, const glm::mat2& val) const {
		UploadMatrix2x2f(name.c_str(), val);
	}

	void HydrogenShader::UploadMatrix3x3f(const char* name, const glm::mat3& val) const {
		glCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val)));
	}

	void HydrogenShader::UploadMatrix3x3f(const std::string& name, const glm::mat3& val) const {
		UploadMatrix3x3f(name.c_str(), val);
	}

	void HydrogenShader::UploadBuffer(const char* name, HydrogenBuffer* buffer) {
		glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Program, name), buffer->GetIdentifier());
	}

	void HydrogenShader::UploadBuffer(const std::string& name, HydrogenBuffer* buffer) {
		UploadBuffer(name.c_str(), buffer);
	}

}