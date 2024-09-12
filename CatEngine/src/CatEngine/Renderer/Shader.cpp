#include "cepch.h"
#include "Shader.h"

#include "CatEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace CatEngine {

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       CE_API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLShader>(name, vertexShader, fragmentShader);
		}
		CE_API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:       CE_API_ASSERT(false, "RendererAPI::NONE is not supported at this time!"); return nullptr;
		case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLShader>(filepath);
		}
		CE_API_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		CE_API_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		CE_API_ASSERT(Exists(name), "Shader not found!");
		return Exists(name) ? m_Shaders[name] : nullptr;
	}
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
