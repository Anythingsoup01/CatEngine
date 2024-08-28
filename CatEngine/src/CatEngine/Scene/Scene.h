#pragma once
#include "CatEngine/Core/TimeStep.h"
#include "CatEngine/Camera/EditorCamera.h"

#include "entt.hpp"

namespace CatEngine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DeleteEntity(Entity entity);

		void OnUpdateRuntime(Time time);
		void OnUpdateEditor(Time time, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}