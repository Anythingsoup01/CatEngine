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

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool ScriptClassExists(const std::string& fullClassName);
		static void OnStartEntity(Entity e);
		static void OnUpdateEntity(Entity e, float ts);

		static Scene* GetSceneContext();
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID entityID);

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

}
