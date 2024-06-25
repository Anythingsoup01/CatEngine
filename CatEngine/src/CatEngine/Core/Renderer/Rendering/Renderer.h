#pragma once

#include "CatEngine/Core/Renderer/Buffers/Buffer.h"

#include "RenderCommand.h"

#include "CatEngine/Core/Renderer/Shader/OpenGLShader.h"

#include "CatEngine/Core/Camera/Camera.h"

namespace CatEngine {

	class Renderer {
	public:
		//inline RendererAPI SetRenderAPI();
		static inline RendererAPI::API GetCurrentAPI() { return RendererAPI::GetAPI() ; }

		static void BeginScene(OrthographicCamera& camera); // TODO Initiate default variables
		static void EndScene();
		
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};

}



