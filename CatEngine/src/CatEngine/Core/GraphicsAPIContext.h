#pragma once

namespace CatEngine {

	class GraphicsAPIContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}