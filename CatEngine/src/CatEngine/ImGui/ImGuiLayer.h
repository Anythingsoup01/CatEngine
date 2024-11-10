#pragma once

#include "CatEngine/Layers/Layer.h"
#include "CatEngine/Events/MouseEvent.h"
#include "CatEngine/Events/KeyEvent.h"
#include "CatEngine/Events/ApplicationEvent.h"


namespace CatEngine {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Time deltaTime) override;
		virtual void OnImGuiDraw() override {};
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		bool BlockKeyboardEvents(bool block) { m_BlockKeyboardEvents = block; return block; }
		bool BlockMouseEvents(bool block) { m_BlockMouseEvents = block; return block; }
		void SetDarkThemeColors();
		void SetDefaultEditorStyle();
	private:
	private:
		bool m_BlockKeyboardEvents = true;
		bool m_BlockMouseEvents = true;
		float m_Time = 0.0;
	};

}
