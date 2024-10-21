#pragma once

#include <filesystem>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}

namespace CatEngine
{
	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filePath);
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		friend class ScriptClass;
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const char* classNamespace, const char* className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const char* methodName, int parameterCount);
	private:
		const char* m_ClassNamespace;
		const char* m_ClassName;

		MonoClass* m_MonoClass = nullptr;

	};
}
