#pragma once

#include <filesystem>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoAssembly MonoAssembly;
}

namespace CatEngine
{
	class Scene;
	class Entity;


	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const char* classNamespace, const char* className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const char* methodName, int parameterCount = 0);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

	private:
		const char* m_ClassNamespace;
		const char* m_ClassName;

		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeUpdateMethod(float ts);
		void InvokeStartMethod();
	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_UpdateMethod = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_StartMethod = nullptr;
	};


	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filePath);
		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);
		static void OnStartEntity(Entity e);
		static void OnUpdateEntity(Entity e, float ts);

		static Scene* GetSceneContext();
		static std::unordered_map<std::string, Ref<ScriptClass>>& GetScriptClasses();
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses(MonoAssembly* assembly);

		static MonoImage* GetCoreImage();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

}
