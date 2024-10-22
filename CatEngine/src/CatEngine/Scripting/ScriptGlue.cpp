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

#define CE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("CatEngine.MeownoBehaviour::" #Name, Name)

	static void Entity_GetPosition(UUID entityID, glm::vec3* outPosition)
	{
		Scene* scene = ScriptEngine::GetSceneContext();

		Entity entity = scene->GetEntityByUUID(entityID);
		*outPosition = entity.GetComponent<TransformComponent>().Position;
	}

	static void Entity_SetPosition(UUID entityID, glm::vec3* position)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		entity.GetComponent<TransformComponent>().Position = *position;
 	}

	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}

	void ScriptGlue::RegisterFunctions()
	{
		CE_ADD_INTERNAL_CALL(Entity_GetPosition);
		CE_ADD_INTERNAL_CALL(Entity_SetPosition);
		CE_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
