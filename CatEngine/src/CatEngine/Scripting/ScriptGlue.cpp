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

#include "CatEngine/Physics/Physics2D.h"


namespace CatEngine
{

	static std::unordered_map < MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define CE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("CatEngine.InternalCalls::" #Name, Name)

	static Entity GetEntity(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CE_ASSERT(entity);
		return entity;
	}

	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}
#pragma region Object

	static bool Object_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Entity& entity = GetEntity(entityID);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		CE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static uint64_t Object_FindObjectByName(MonoString* name)
	{
		char* cStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		CE_ASSERT(scene);
		Entity entity = scene->FindEntityByName(cStr);
		CE_ASSERT(entity);
		mono_free(cStr);
		
		if (!entity)
			return 0;

		return entity.GetUUID();

	}

#pragma endregion

#pragma region Scripts

	static MonoObject* GetScriptInstance(UUID entityID)
	{
		return ScriptEngine::GetManagedInstance(entityID);
	}

#pragma endregion


#pragma region Rigidbody2D

	static void Rigidbody2D_ApplyForce(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Entity& entity = GetEntity(entityID);
		Physics2D::ApplyForce(entity, *impulse, *point, wake);
	}
	static void Rigidbody2D_ApplyForceToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Entity& entity = GetEntity(entityID);
		Physics2D::ApplyForceToCenter(entity, *impulse, wake);
	}
	static void Rigidbody2D_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Entity& entity = GetEntity(entityID);
		Physics2D::ApplyLinearImpulse(entity, *impulse, *point, wake);
	}
	static void Rigidbody2D_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Entity& entity = GetEntity(entityID);
		Physics2D::ApplyLinearImpulseToCenter(entity, *impulse, wake);
	}


#pragma endregion

#pragma region Transform

	static void Transform_GetPosition(UUID entityID, glm::vec3* outPosition)
	{
		Entity& entity = GetEntity(entityID);
		*outPosition = entity.GetComponent<TransformComponent>().Position;
	}

	static void Transform_SetPosition(UUID entityID, glm::vec3* position)
	{
		Entity& entity = GetEntity(entityID);

		entity.GetComponent<TransformComponent>().Position = *position;
	}

	static void Transform_GetRotation(UUID entityID, glm::vec3* outRotation)
	{
		Entity& entity = GetEntity(entityID);
		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void Transform_SetRotation(UUID entityID, glm::vec3* rotation)
	{
		Entity& entity = GetEntity(entityID);

		entity.GetComponent<TransformComponent>().Rotation = *rotation;
	}

	static void Transform_GetScale(UUID entityID, glm::vec3* outScale)
	{
		Entity& entity = GetEntity(entityID);
		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void Transform_SetScale(UUID entityID, glm::vec3* scale)
	{
		Entity& entity = GetEntity(entityID);

		entity.GetComponent<TransformComponent>().Scale = *scale;
	}

#pragma endregion


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
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		CE_ADD_INTERNAL_CALL(Input_IsKeyDown);

#pragma region Object

		CE_ADD_INTERNAL_CALL(Object_HasComponent);
		CE_ADD_INTERNAL_CALL(Object_FindObjectByName);


#pragma endregion

#pragma region Scripts

		CE_ADD_INTERNAL_CALL(GetScriptInstance);

#pragma endregion


#pragma region Rigidbody2D
		CE_ADD_INTERNAL_CALL(Rigidbody2D_ApplyForce);
		CE_ADD_INTERNAL_CALL(Rigidbody2D_ApplyForceToCenter);
		CE_ADD_INTERNAL_CALL(Rigidbody2D_ApplyLinearImpulse);
		CE_ADD_INTERNAL_CALL(Rigidbody2D_ApplyLinearImpulseToCenter);
#pragma endregion

#pragma region Transform
		CE_ADD_INTERNAL_CALL(Transform_GetPosition);
		CE_ADD_INTERNAL_CALL(Transform_SetPosition);
		CE_ADD_INTERNAL_CALL(Transform_GetRotation);
		CE_ADD_INTERNAL_CALL(Transform_SetRotation);
		CE_ADD_INTERNAL_CALL(Transform_GetScale);
		CE_ADD_INTERNAL_CALL(Transform_SetScale);
#pragma endregion

	}
}
