#include "cepch.h"
#include "ScriptGlue.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include "CatEngine/Math/Math.h"

namespace CatEngine
{

#define CE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("CatEngine.InternalCalls::" #Name, Name)

	static void NativeLogString(MonoString* text, int parameter)
	{
		char* cStr = mono_string_to_utf8(text);
		std::string str(cStr);
		mono_free(cStr);
		CE_CLI_INFO("{0}-{1}", str, parameter);
	}

	static void NativeLogVector3(Math::Vector3* parameter)
	{
		CE_CLI_INFO("Vec3 - {0}", *parameter);
	}

	static void NativeLogVector2(Math::Vector2* parameter)
	{
		CE_CLI_INFO("Vec2 - {0}", *parameter);
	}

	static Math::Vector3 AddVector3(Math::Vector3* value1, Math::Vector3* value2)
	{

		return *value1 += *value2;
	}

	void ScriptGlue::RegisterFunctions()
	{
		CE_ADD_INTERNAL_CALL(NativeLogString);
		CE_ADD_INTERNAL_CALL(NativeLogVector2);
		CE_ADD_INTERNAL_CALL(NativeLogVector3);
		CE_ADD_INTERNAL_CALL(AddVector3);
	}
}
