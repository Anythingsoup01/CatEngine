#pragma once
#include "entt.hpp"
#include "CatEngine/Core/TimeStep.h"


namespace CatEngine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Time time);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}