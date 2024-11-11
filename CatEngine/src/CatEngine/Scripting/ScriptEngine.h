#pragma once

#include <filesystem>

#include "CatEngine/Core/UUID.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoAssembly MonoAssembly;
}

namespace CatEngine
{

	class Scene;
	class Entity;

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		SByte, Char, Int16, Int32, Int64, Boolean,
		Byte, UInt16, UInt32, UInt64,
		String,
		Vector2, Vector3, Vector4,
		Object, Component, MeownoBehaviour,
		TransformComponent, Rigidbody2DComponent,
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;
		MonoClassField* ClassField;
	};

	struct ScriptFieldInstance
	{
		ScriptField Field;
		ScriptFieldInstance()
		{
			memset(m_Data, 0, sizeof(m_Data));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 128, "Type to large!");
			return *(T*)m_Data;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 128, "Type to large!");
			memcpy(m_Data, &value, sizeof(T));
		}


	private:

		char m_Data[128];

		friend class ScriptEngine;
		friend class ScriptInstance;
	};


	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const char* classNamespace, const char* className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const char* methodName, int parameterCount = 0);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		const std::unordered_map<std::string, ScriptField>& GetFields() const { return m_Fields; }

	private:
		const char* m_ClassNamespace;
		const char* m_ClassName;

		std::unordered_map<std::string, ScriptField> m_Fields;

		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
		friend class ScriptInstance;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeUpdateMethod(float ts);
		void InvokeStartMethod();

		Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetFieldData(const std::string& name)
		{
			static_assert(sizeof(T) <= 128, "Type to large!");
			bool success = GetFieldDataInternal(name, &s_FieldValueBuffer);
			if (!success)
				return T();
			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldData(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= 128, "Type to large!");
			SetFieldDataInternal(name, value);
		}

		MonoObject* GetManagedObject() { return m_Instance; }

	private:
		bool GetFieldDataInternal(const std::string& name, void* buffer);
		void SetFieldDataInternal(const std::string& name, void* value);
	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_UpdateMethod = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_StartMethod = nullptr;

		static inline char s_FieldValueBuffer[8];

		friend class ScriptEngine;
		friend struct ScriptFieldInstance;
	};


	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filePath);
		static void LoadAppAssembly(const std::filesystem::path& filePath);

		static void SetSceneContext(Ref<Scene> scene);
		static void OnRuntimeStop();

		static void ReloadAssembly();

		static bool ScriptClassExists(const std::string& fullClassName);
		static void OnStartEntity(Entity e);
		static void OnUpdateEntity(Entity e, float ts);

		static Ref<Scene> GetSceneContext();
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID entityID);

		static MonoObject* GetManagedInstance(UUID uuid);

		static std::unordered_map<std::string, Ref<ScriptClass>>& GetScriptClasses();
		static Ref<ScriptClass> GetScriptClass(const std::string& name);
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);


		static MonoImage* GetCoreImage();
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();


		friend class ScriptClass;
		friend class ScriptGlue;
	};

	namespace Utils
	{
		static const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
		{
			switch (fieldType)
			{
			case ScriptFieldType::None:					return "None";
			case ScriptFieldType::Float:					return "Float";
			case ScriptFieldType::Double:					return "Double";
			case ScriptFieldType::SByte:					return "SByte";
			case ScriptFieldType::Char:						return "Char";
			case ScriptFieldType::Int16:					return "Int16";
			case ScriptFieldType::Int32:					return "Int32";
			case ScriptFieldType::Int64:					return "Int64";
			case ScriptFieldType::Boolean:					return "Bool";
			case ScriptFieldType::Byte:						return "Byte";
			case ScriptFieldType::UInt16:					return "UInt16";
			case ScriptFieldType::UInt32:					return "UInt32";
			case ScriptFieldType::UInt64:					return "UInt64";
			case ScriptFieldType::String:					return "String";
			case ScriptFieldType::Vector2:					return "Vector2";
			case ScriptFieldType::Vector3:					return "Vector3";
			case ScriptFieldType::Vector4:					return "Vector4";
			case ScriptFieldType::Object:					return "Object";
			case ScriptFieldType::Component:				return "Component";
			case ScriptFieldType::MeownoBehaviour:			return "MeownoBehaviour";
			case ScriptFieldType::TransformComponent:		return "TransformComponent";
			case ScriptFieldType::Rigidbody2DComponent:		return "Rigidbody2DComponent";
			}
			CE_API_ASSERT(false, "Unknown Field Type!");
			return "None";
		}
		static ScriptFieldType StringToScriptFieldType(std::string_view fieldType)
		{
			if (fieldType == "Float")					return ScriptFieldType::Float;
			if (fieldType == "Double")					return ScriptFieldType::Double;
			if (fieldType == "SByte")					return ScriptFieldType::SByte;
			if (fieldType == "Char")					return ScriptFieldType::Char;
			if (fieldType == "Int16")					return ScriptFieldType::Int16;
			if (fieldType == "Int32")					return ScriptFieldType::Int32;
			if (fieldType == "Int64")					return ScriptFieldType::Int64;
			if (fieldType == "Boolean")					return ScriptFieldType::Boolean;
			if (fieldType == "Byte")					return ScriptFieldType::Byte;
			if (fieldType == "UInt16")					return ScriptFieldType::UInt16;
			if (fieldType == "UInt32")					return ScriptFieldType::UInt32;
			if (fieldType == "UInt64")					return ScriptFieldType::UInt64;
			if (fieldType == "String")					return ScriptFieldType::String;
			if (fieldType == "Vector2")					return ScriptFieldType::Vector2;
			if (fieldType == "Vector3")					return ScriptFieldType::Vector3;
			if (fieldType == "Vector4")					return ScriptFieldType::Vector4;
			if (fieldType == "Object")					return ScriptFieldType::Object;
			if (fieldType == "Component")				return ScriptFieldType::Component;
			if (fieldType == "MeownoBehaviour")			return ScriptFieldType::MeownoBehaviour;
			if (fieldType == "TransformComponent")		return ScriptFieldType::TransformComponent;
			if (fieldType == "Rigidbody2DComponent")	return ScriptFieldType::Rigidbody2DComponent;
			return ScriptFieldType::None;
		}
		
	}
}
