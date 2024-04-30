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

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Events& event);
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0;
	};

}