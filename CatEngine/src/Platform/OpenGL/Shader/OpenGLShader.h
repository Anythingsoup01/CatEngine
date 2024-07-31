#pragma once

#include "CatEngine/Core/Shader/Shader.h"
#include <glm/glm.hpp>


#include <glad/glad.h>

namespace CatEngine {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat2(const std::string& name, const glm::mat2& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual void SetInt(const std::string& name, int value) override;

		virtual void SetVec1(const std::string& name, const glm::vec1& value) override;
		virtual void SetVec1(const std::string& name, float value) override;
		virtual void SetVec2(const std::string& name, const glm::vec2& value) override;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) override;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) override;

		virtual std::string GetName() const override;

		void UploadUniformMat2(const std::string& name, const glm::mat2& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& value);

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformVector4(const std::string& name, const glm::vec4& value);
		void UploadUniformVector3(const std::string& name, const glm::vec3& value);
		void UploadUniformVector2(const std::string& name, const glm::vec2& value);
		void UploadUniformVector1(const std::string& name, const glm::vec1& value);


	private:
		inline GLint GetLocation(const std::string& name);
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<unsigned int, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<unsigned int, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;

		std::unordered_map<GLint, std::string> m_Locations;
	};
}
