#include "cepch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include "ScriptGlue.h"

#include "CatEngine/Math/Math.h"

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
			uint32_t size = end - stream.tellg();

			if (size == 0)
				CE_API_CRITICAL("File - {0} - is empty!");

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
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

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
		LoadAssembly("Resources/Scripts/Cat-ScriptCore.dll");

		ScriptGlue::RegisterFunctions();

		s_Data->EntityScriptClass = ScriptClass("CatEngine", "Entity");
		s_Data->EntityClassInstance = s_Data->EntityScriptClass.Instantiate();

		// Put specific events here - i.e. Start, Update, Awake, Collision, Possibly Triggers
		s_Data->OnStartMethod = s_Data->EntityScriptClass.GetMethod("Start", 0);
		if (s_Data->OnStartMethod) mono_runtime_invoke(s_Data->OnStartMethod, s_Data->EntityClassInstance, nullptr, nullptr);

		s_Data->OnUpdateMethod = s_Data->EntityScriptClass.GetMethod("Update", 0);

	}
	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filePath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain("CatScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filePath);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);


		//PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("CatEngineJITRuntime");
		CE_API_ASSERT(rootDomain, "Could not load Root Domain \"CatEngineJITRuntime\"");

		s_Data->RootDomain = rootDomain;

	}
	void ScriptEngine::ShutdownMono()
	{
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->AppDomain = nullptr;
		s_Data->RootDomain = nullptr;
	}


	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}
	///////////////////////////////////////////////////////
	// SCRIPT CLASS ///////////////////////////////////////
	///////////////////////////////////////////////////////

	ScriptClass::ScriptClass(const char* classNamespace, const char* className)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(s_Data->CoreAssemblyImage, classNamespace, className);
	}
	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}
	MonoMethod* ScriptClass::GetMethod(const char* methodName, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, methodName, parameterCount);
	}
}
