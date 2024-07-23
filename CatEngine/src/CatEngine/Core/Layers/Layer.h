#pragma once

#include "CatEngine/Events/Events.h"
#include "CatEngine/Core/TimeStep.h"

namespace CatEngine {
	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Time deltaTime) {}
		virtual void OnImGuiDraw() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}