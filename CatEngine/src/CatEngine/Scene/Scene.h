#pragma once

#include "CatEngine/Camera/EditorCamera.h"
#include "CatEngine/Core/TimeStep.h"
#include "CatEngine/Core/UUID.h"


#include "entt.hpp"

class b2World;

namespace CatEngine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);


		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DeleteEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateRuntime(Time time);
		void OnUpdateSimulation(Time time, EditorCamera& camera);
		void OnUpdateEditor(Time time, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);
		Entity PasteEntity(Entity entity);

		Entity GetEntityByUUID(UUID entityID);

		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetAllComponentsWith()
		{
			return m_Registry.view<Components...>();
		}

		bool IsRunning() { return m_IsRunning; }

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void OnScriptStart();

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		bool m_IsRunning = false;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
