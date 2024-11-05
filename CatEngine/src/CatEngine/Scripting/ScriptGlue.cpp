#include "cepch.h"
#include "ScriptGlue.h"
#include "CatEngine/Core/UUID.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include "CatEngine/Core/Input.h"
#include "CatEngine/Core/KeyCodes.h"
#include "CatEngine/Core/MouseButtonCodes.h"

#include "CatEngine/Math/Math.h"
#include "CatEngine/Scene/Scene.h"
#include "CatEngine/Scene/Entity.h"
#include "ScriptEngine.h"

namespace CatEngine
{

	static std::unordered_map < MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define CE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("CatEngine.MeownoBehaviour::" #Name, Name)

	static void Transform_GetPosition(UUID entityID, glm::vec3* outPosition)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CE_ASSERT(entity);
		*outPosition = entity.GetComponent<TransformComponent>().Position;
	}

	static void Transform_SetPosition(UUID entityID, glm::vec3* position)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Position = *position;
 	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		CE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}
	template<typename ... Component>
	static void RegisterComponent()
	{

		([]()
		{
			std::string_view fullTypeName = typeid(Component).name();
		size_t pos = fullTypeName.find_last_of(":");
			std::string_view typeName = fullTypeName.substr(pos + 1);
		pos = typeName.size() - sizeof("Component");
			std::string_view componentName = typeName.substr(0, pos + 1);
			std::string managedTypename = fmt::format("CatEngine.{}", componentName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreImage());
			if (!managedType)
			{
				CE_API_ERROR("Could not find component type {}", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename ... Component>
	static void RegisterComponent(ComponentGroup<Component ...>)
	{
		RegisterComponent<Component ...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		CE_ADD_INTERNAL_CALL(Transform_GetPosition);
		CE_ADD_INTERNAL_CALL(Transform_SetPosition);
		CE_ADD_INTERNAL_CALL(Entity_HasComponent);
		CE_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
