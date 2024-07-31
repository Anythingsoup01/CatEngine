#include "cepch.h"
#include "OpenGLShader.h"

#include "CatEngine/Core/Logging/Log.h"

#include <fstream>

#include "glm/gtc/type_ptr.hpp"

#include "CatEngine/Core/Timer.h"

namespace CatEngine {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		API_ASSERT(false, "Unknown shader type '{0}'", type);
		return -1;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		m_Name = name;
	}
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// assets/shaders/Texture.glsl
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::fstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			CLI_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			API_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			API_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified!");

			size_t nextLinePos = source.find_first_not_of("/r/n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		API_ASSERT(shaderSources.size() <= 2, "CatEngine Only Supports 2 Shaders at this moment!");
		int glShaderIDIndex = 0;
		std::array<GLenum, 2> glShaderIDs;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* srcCStr = source.c_str();
			glShaderSource(shader, 1, &srcCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				API_ERROR("{0}", infoLog.data());
				API_ASSERT(false, "'{0}' compilation failure!", type);
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the m_RendererID anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			API_ERROR("{0}", infoLog.data());
			API_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glDetachShader(m_RendererID, id);
			glDeleteShader(id);
		}
		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& value)
	{
		UploadUniformMat2(name, value);
	}
	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		UploadUniformMat3(name, value);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}
	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}
	void OpenGLShader::SetVec1(const std::string& name, const glm::vec1& value)
	{
		UploadUniformVector1(name, value);
	}
	void OpenGLShader::SetVec1(const std::string& name, float value)
	{
		UploadUniformVector1(name, (glm::vec1)value);
	}
	void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformVector2(name, value);
	}
	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformVector3(name, value);
	}
	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformVector4(name, value);
	}

	std::string OpenGLShader::GetName() const
	{
		return m_Name;
	}

	void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& value)
	{
		glUniformMatrix2fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		glUniform1i(GetLocation(name), value);
	}

	void OpenGLShader::UploadUniformVector4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(GetLocation(name), value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::UploadUniformVector3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetLocation(name), value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformVector2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetLocation(name), value.x, value.y);
	}
	void OpenGLShader::UploadUniformVector1(const std::string& name, const glm::vec1& value)
	{
		glUniform1f(GetLocation(name), value.x);
	}

	GLint OpenGLShader::GetLocation(const std::string& name)
	{
		for (auto& item : m_Locations)
		{
			if (item.second == name)
			{
				return item.first;
			}
		}
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_Locations.insert(std::pair<GLint, std::string>(location, name));
		return location;
	}

}