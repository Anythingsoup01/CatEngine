#include "cepch.h"
#include "Scene.h"

#include "Components.h"

#include "CatEngine/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace CatEngine
{
	static void DoMath(const glm::mat4& transform)
	{

	}


	Scene::Scene()
	{

		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.f));
		if (m_Registry.try_get<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		}
	}

	Scene::~Scene()
	{

	}
	void Scene::OnUpdate(Time time)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
}