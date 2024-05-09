#pragma once

#include <string>

namespace CatEngine {
	class OpenGLShader
	{
	public:
		OpenGLShader(const std::string& vertexShader, const std::string& fragmentShader);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;
	};
}
