#include "cepch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include "ScriptGlue.h"

#include "CatEngine/Math/Math.h"
#include "CatEngine/Scene/Scene.h"
#include "CatEngine/Scene/Entity.h"

namespace CatEngine
{

	namespace Utils
	{

		// Todo - Move to a filesystem class
		static char* ReadBytes(const std::filesystem::path& filePath, uint32_t* outSize)
		{
			std::ifstream stream(filePath, std::ios::binary | std::ios::ate);

			if (!stream)
				CE_API_CRITICAL("Could not open file");

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint64_t size = end - stream.tellg();

			if (size == 0)
				CE_API_CRITICAL("File - {0} - is empty!");

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = (uint32_t)size;
			return buffer;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				CE_API_TRACE("{}.{}", nameSpace, name);
			}
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& filePath)
		{
			uint32_t fileSize = 0;
			std::string& path = filePath.string();
			char* fileData = ReadBytes(filePath, &fileSize);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				CE_API_CRITICAL(errorMessage);
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, path.c_str(), &status, 0);
			mono_image_close(image);

			delete[] fileData;

			return assembly;
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoObject* EntityClassInstance = nullptr;

		MonoMethod* OnStartMethod = nullptr;
		MonoMethod* OnUpdateMethod = nullptr;

		ScriptClass EntityScriptClass;
	};

	static ScriptEngineData* s_ScriptData = nullptr;

	void ScriptEngine::Init()
	{
		s_ScriptData = new ScriptEngineData();

		InitMono();
		LoadAssembly("Resources/Scripts/Cat-ScriptCore.dll");

		ScriptGlue::RegisterFunctions();

		s_ScriptData->EntityScriptClass = ScriptClass("CatEngine", "Entity");
		s_ScriptData->EntityClassInstance = s_ScriptData->EntityScriptClass.Instantiate();

		// Put specific events here - i.e. Start, Update, Awake, Collision, Possibly Triggers
		s_ScriptData->OnStartMethod = s_ScriptData->EntityScriptClass.GetMethod("Start");
		if (s_ScriptData->OnStartMethod) s_ScriptData->EntityScriptClass.InvokeMethod(s_ScriptData->EntityClassInstance, s_ScriptData->OnStartMethod);

		s_ScriptData->OnUpdateMethod = s_ScriptData->EntityScriptClass.GetMethod("Update");

	}
	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_ScriptData;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filePath)
	{
		s_ScriptData->AppDomain = mono_domain_create_appdomain("CatScriptRuntime", nullptr);
		mono_domain_set(s_ScriptData->AppDomain, true);

		s_ScriptData->CoreAssembly = Utils::LoadMonoAssembly(filePath);
		s_ScriptData->CoreAssemblyImage = mono_assembly_get_image(s_ScriptData->CoreAssembly);


		//PrintAssemblyTypes(s_ScriptData->CoreAssembly);
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("CatEngineJITRuntime");
		CE_API_ASSERT(rootDomain, "Could not load Root Domain \"CatEngineJITRuntime\"");

		s_ScriptData->RootDomain = rootDomain;

	}
	void ScriptEngine::ShutdownMono()
	{
		mono_jit_cleanup(s_ScriptData->RootDomain);
		s_ScriptData->AppDomain = nullptr;
		s_ScriptData->RootDomain = nullptr;
	}


	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_ScriptData->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}
	///////////////////////////////////////////////////////
	// SCRIPT CLASS ///////////////////////////////////////
	///////////////////////////////////////////////////////

	ScriptClass::ScriptClass(const char* classNamespace, const char* className)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(s_ScriptData->CoreAssemblyImage, classNamespace, className);
	}
	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}
	MonoMethod* ScriptClass::GetMethod(const char* methodName, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, methodName, parameterCount);
	}
	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	///////////////////////////////////////////////////////
	// SCRIPT INSTANCE ////////////////////////////////////
	///////////////////////////////////////////////////////

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{ 
		// Put specific events here - i.e. Start, Update, Awake, Collision, Possibly Triggers
		m_Instance = scriptClass->Instantiate();

		m_Constructor = s_ScriptData->MeownoClass.GetMethod(".ctor", 1);
		m_StartMethod = scriptClass->GetMethod("Start");
		m_UpdateMethod = scriptClass->GetMethod("Update", 1);

		{
			UUID entityID = entity.GetUUID();
			void* params = &entityID;
			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &params);
		}
	}

	void ScriptInstance::InvokeUpdateMethod(float ts)
	{
		void* param = &ts;
		m_ScriptClass->InvokeMethod(m_Instance, m_UpdateMethod, &param);
	}

	void ScriptInstance::InvokeStartMethod()
	{
		m_ScriptClass->InvokeMethod(m_Instance, m_StartMethod);
	}
}
