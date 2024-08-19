#pragma once
#include "entt.hpp"
#include "CatEngine/Core/TimeStep.h"


namespace CatEngine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// TEMP

		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Time time);
	private:
		entt::registry m_Registry;
	};
}