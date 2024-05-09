#pragma once
namespace CatEngine {
	
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
	};

	class Renderer {
	public:
		//inline RendererAPI SetRenderAPI();
		static inline RendererAPI GetCurrentAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};

}



