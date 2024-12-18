#pragma once

#include "CatEngine/Renderer/Buffer.h"

#include "RenderCommand.h"

#include "CatEngine/Renderer/Shader.h"

#include "CatEngine/Camera/Camera.h"

namespace CatEngine {

	class Renderer {
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);


		static void BeginScene(OrthographicCamera& camera); // TODO Initiate default variables
		static void EndScene();
		
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI::API GetCurrentAPI() { return RendererAPI::GetAPI() ; }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* s_SceneData;
	};

}



