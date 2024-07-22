#pragma once

#include "CatEngine/Core/Renderer/Buffers/Buffer.h"

#include "RenderCommand.h"

#include "CatEngine/Core/Renderer/Shader/Shader.h"

#include "CatEngine/Core/Camera/Camera.h"

namespace CatEngine {

	class Renderer {
	public:
		static void Init();

		//inline RendererAPI SetRenderAPI();
		static inline RendererAPI::API GetCurrentAPI() { return RendererAPI::GetAPI() ; }

		static void BeginScene(OrthographicCamera& camera); // TODO Initiate default variables
		static void EndScene();
		
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};

}



