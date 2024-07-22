#pragma once

#include "CatEngine/Core/Renderer/Shader/Shader.h"
// TODO -- REMOVE THE NEED FOR THIS (CAN"T INCLUDE GLAD DUE TO DYNAMIC CASTING IN SANDBOXAPP)
typedef unsigned int GLenum;
#include <glm/glm.hpp>

namespace CatEngine {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual std::string GetName() const override;

		void UploadUniformMat4(const std::string name, const glm::mat4& matrix);
		void UploadUniformMat3(const std::string name, const glm::mat3& matrix);

		void UploadUniformInt(const std::string name, int integer);

		void UploadUniformVector4(const std::string name, const glm::vec4& vector);
		void UploadUniformVector3(const std::string name, const glm::vec3& vector);
		void UploadUniformVector2(const std::string name, const glm::vec2& vector);
		void UploadUniformVector1(const std::string name, const glm::vec1& vector);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}


