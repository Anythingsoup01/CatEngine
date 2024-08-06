#pragma once

#include "CatEngine/Core/Layers/Layer.h"
#include "CatEngine/Events/MouseEvent.h"
#include "CatEngine/Events/KeyEvent.h"
#include "CatEngine/Events/ApplicationEvent.h"


namespace CatEngine {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Time deltaTime) override {};
		virtual void OnImGuiDraw() override {};
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0;
	};

}