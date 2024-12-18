#include "cepch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"

#include "FileWatch.hpp"

#include "ScriptGlue.h"

#include "CatEngine/Math/Math.h"
#include "CatEngine/Scene/Scene.h"
#include "CatEngine/Scene/Entity.h"

#include "CatEngine/Core/Timer.h"
#include "CatEngine/Core/Buffer.h"
#include "CatEngine/Core/Filesystem.h"

#include "CatEngine/Application.h"

namespace CatEngine
{
	std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{"System.Single", ScriptFieldType::Float},
		{"System.Double", ScriptFieldType::Double},
		{"System.SByte",ScriptFieldType::SByte},
		{"System.Char",ScriptFieldType::Char},
		{"System.Int16", ScriptFieldType::Int16},
		{"System.Int32", ScriptFieldType::Int32},
		{"System.Int64", ScriptFieldType::Int64},
		{"System.Boolean", ScriptFieldType::Boolean},
		{"System.Byte", ScriptFieldType::Byte},
		{"System.UInt16", ScriptFieldType::UInt16},
		{"System.UInt32", ScriptFieldType::UInt32},
		{"System.UInt64", ScriptFieldType::UInt64},
		{"System.String", ScriptFieldType::String},
		{"CatEngine.Vector2", ScriptFieldType::Vector2},
		{"CatEngine.Vector3", ScriptFieldType::Vector3},
		{"CatEngine.Vector4", ScriptFieldType::Vector4},
		{"CatEngine.Object", ScriptFieldType::Object},
		{"CatEngine.Component", ScriptFieldType::Component},
		{"CatEngine.MeownoBehaviour", ScriptFieldType::MeownoBehaviour},
		{"CatEngine.Transform", ScriptFieldType::TransformComponent},
		{"CatEngine.Rigidbody2D", ScriptFieldType::Rigidbody2DComponent},

	};

	namespace Utils
	{
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& filePath, bool loadPDB = false)
		{
			ScopedBuffer fileData(FileSystem::ReadFileBinary(filePath));

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full((char*)fileData.Data(), (uint32_t)fileData.Size(), 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				CE_API_CRITICAL(errorMessage);
				return nullptr;
			}

			if (loadPDB)
			{
				std::filesystem::path pdbPath = filePath;
				pdbPath.replace_extension(".pdb");

				if (std::filesystem::exists(pdbPath))
				{
					ScopedBuffer pdbFileData = FileSystem::ReadFileBinary(pdbPath);
					mono_debug_open_image_from_memory(image, pdbFileData.As<const mono_byte>(), (uint32_t)pdbFileData.Size());
					CE_API_INFO("Loaded PDB {}", pdbPath.string());
				}

			}

			std::string pathString = filePath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
			mono_image_close(image);

			return assembly;
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

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);
			auto& it = s_ScriptFieldTypeMap.find(typeName);

			if (it != s_ScriptFieldTypeMap.end())
				return it->second;

			CE_API_ERROR("Not supported type {}", typeName);
			return ScriptFieldType::None;
		}
	}

	

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		std::filesystem::path CoreAssemblyFilepath;
		std::filesystem::path AppAssemblyFilepath;

		ScriptClass MeownoClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;

		std::unordered_map<UUID, ScriptFieldMap> EntityScriptFields;

		Scope<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;

		std::vector <std::function<void()>> MainThreadQueue;

		bool AppAssemblyReloadPending = false;

		bool FunctionRegistered = false;

		Ref<Scene> SceneContext = nullptr;

		Timer ReloadTimer;
	};

	static ScriptEngineData* s_ScriptData = nullptr;

	void ScriptEngine::Init()
	{
		s_ScriptData = new ScriptEngineData();

		InitMono();
		bool status = LoadAssembly("Resources/Scripts/Cat-ScriptCore.dll");
		if (!status)
		{
			CE_API_ERROR("[Script Engine] - Could not load Core Assembly!");
			return;
		}
		status = LoadAppAssembly("SampleProject/Assets/Scripts/Binaries/AssemblyC#.dll");
		if (!status)
		{
			CE_API_ERROR("[Script Engine] - Could not load App Assembly!");
			return;
		}
		LoadAssemblyClasses();
		//Utils::PrintAssemblyTypes(s_ScriptData->CoreAssembly);

		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		s_ScriptData->FunctionRegistered = true;

		s_ScriptData->MeownoClass = ScriptClass("CatEngine", "Object", true);
	}
	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_ScriptData;
	}

	bool ScriptEngine::LoadAssembly(const std::filesystem::path& filePath)
	{
		s_ScriptData->AppDomain = mono_domain_create_appdomain("CatScriptRuntime", nullptr);

		if (!s_ScriptData->AppDomain)
			return false;

		mono_domain_set(s_ScriptData->AppDomain, true);
		
		s_ScriptData->CoreAssemblyFilepath = filePath;
		s_ScriptData->CoreAssembly = Utils::LoadMonoAssembly(filePath);
		s_ScriptData->CoreAssemblyImage = mono_assembly_get_image(s_ScriptData->CoreAssembly);
		return true;

	}

	static void OnAppAssemblyFileSystemEvent(const std::string& path, const filewatch::Event change_type)
	{
		
		if (change_type == filewatch::Event::modified)
		{
			bool isRunning = s_ScriptData->SceneContext->IsRunning();

			s_ScriptData->ReloadTimer = Timer();

			if (isRunning)
			{
				if (!s_ScriptData->AppAssemblyReloadPending)
				{
					s_ScriptData->AppAssemblyReloadPending = true;
					s_ScriptData->MainThreadQueue.emplace_back([]()
					{
						s_ScriptData->AppAssemblyFileWatcher.reset();
						ScriptEngine::ReloadAssembly();
					});
				}
				else if (s_ScriptData->AppAssemblyReloadPending)
				{
					s_ScriptData->MainThreadQueue.clear();
					s_ScriptData->MainThreadQueue.emplace_back([]()
					{
						s_ScriptData->AppAssemblyFileWatcher.reset();
						ScriptEngine::ReloadAssembly();
					});
				}

			}
			else if (!isRunning)
			{
				using namespace std::chrono;
				//std::this_thread::sleep_for(500ms);
				if (!s_ScriptData->AppAssemblyReloadPending)
				{
					s_ScriptData->AppAssemblyReloadPending = true;
					Application::Get().SubmitToMainThread([]()
					{
						s_ScriptData->AppAssemblyFileWatcher.reset();
						ScriptEngine::ReloadAssembly();
					});
				}

			}

		}


	}

	bool ScriptEngine::LoadAppAssembly(const std::filesystem::path& filePath)
	{
		s_ScriptData->AppAssemblyFilepath = filePath;
		s_ScriptData->AppAssembly = Utils::LoadMonoAssembly(filePath);

		if (!s_ScriptData->AppAssembly)
			return false;

		s_ScriptData->AppAssemblyImage = mono_assembly_get_image(s_ScriptData->AppAssembly);

		s_ScriptData->AppAssemblyFileWatcher = CreateScope<filewatch::FileWatch<std::string>>(
			filePath.string(), OnAppAssemblyFileSystemEvent);

		return true;
	}

	void ScriptEngine::SetSceneContext(Ref<Scene> scene)
	{
		s_ScriptData->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_ScriptData->SceneContext = nullptr;
		s_ScriptData->EntityInstances.clear();
		
		if (s_ScriptData->AppAssemblyReloadPending || !s_ScriptData->MainThreadQueue.empty())
			Application::Get().SubmitToMainThread(s_ScriptData->MainThreadQueue.at(0));
	}

	void ScriptEngine::ReloadAssembly()
	{
#ifdef CE_RELEASE
		CE_CLI_TRACE(s_ScriptData->ReloadTimer.ElapsedMillis());
#endif
		CE_API_TRACE(s_ScriptData->ReloadTimer.ElapsedMillis());
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_ScriptData->AppDomain);
		bool status = LoadAssembly(s_ScriptData->CoreAssemblyFilepath);
		if (!status)
		{
			CE_API_ERROR("[Script Engine] - Could not load Core Assembly!");
			return;
		}
		status = LoadAppAssembly(s_ScriptData->AppAssemblyFilepath);
		if (!status)
		{
			CE_API_ERROR("[Script Engine] - Could not load App Assembly!");
			return;
		}
		LoadAssemblyClasses();


		ScriptGlue::RegisterComponents();

		if (!s_ScriptData->FunctionRegistered)
		{
			ScriptGlue::RegisterFunctions();
			s_ScriptData->FunctionRegistered = true;
		}

		s_ScriptData->MeownoClass = ScriptClass("CatEngine", "Object", true);
		s_ScriptData->AppAssemblyReloadPending = false;
	}

	bool ScriptEngine::ScriptClassExists(const std::string& fullClassName)
	{
		return s_ScriptData->EntityClasses.find(fullClassName) != s_ScriptData->EntityClasses.end();
	}

	void ScriptEngine::OnStartEntity(Entity e)
	{

		const auto& sc = e.GetComponent<ScriptComponent>();

		if (ScriptEngine::ScriptClassExists(sc.ClassName))
		{
			UUID entityID = e.GetUUID();
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_ScriptData->EntityClasses[sc.ClassName], e);
			s_ScriptData->EntityInstances[entityID] = instance;

			// Copy field value
			if (s_ScriptData->EntityScriptFields.find(entityID) != s_ScriptData->EntityScriptFields.end())
			{
				const ScriptFieldMap& scriptFieldMap = s_ScriptData->EntityScriptFields.at(entityID);

				for (const auto& [name, fieldInstance] : scriptFieldMap)
				{
					instance->SetFieldDataInternal(name, (void*)fieldInstance.m_Data);
				}
			}

			instance->InvokeStartMethod();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity e, float ts)
	{
		UUID entityUUID = e.GetUUID();

		if (s_ScriptData->EntityInstances.find(entityUUID) != s_ScriptData->EntityInstances.end())
		{
			Ref<ScriptInstance> instance = s_ScriptData->EntityInstances[entityUUID];
			instance->InvokeUpdateMethod(ts);
		}
		else
		{
			CE_API_ERROR("Could not find script instance {}", (uint64_t)entityUUID);
		}

	}

	Ref<Scene> ScriptEngine::GetSceneContext()
	{
		return s_ScriptData->SceneContext;
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID entityID)
	{
		auto& it = s_ScriptData->EntityInstances.find(entityID);
		if (it == s_ScriptData->EntityInstances.end())
			return nullptr;

		return it->second;
	}

	MonoObject* ScriptEngine::GetManagedInstance(UUID uuid)
	{
		if(s_ScriptData->EntityInstances.find(uuid) != s_ScriptData->EntityInstances.end())
			return s_ScriptData->EntityInstances.at(uuid)->GetManagedObject();

		return nullptr;
	}

	std::unordered_map<std::string, Ref<ScriptClass>>& ScriptEngine::GetScriptClasses()
	{
		return s_ScriptData->EntityClasses;
	}

	Ref<ScriptClass> ScriptEngine::GetScriptClass(const std::string& name)
	{
		if (s_ScriptData->EntityClasses.find(name) != s_ScriptData->EntityClasses.end())
			return s_ScriptData->EntityClasses.at(name);

		return nullptr;
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity)
	{
		
		if (entity)
		{
			UUID entityID = entity.GetUUID();
			return s_ScriptData->EntityScriptFields[entityID];
		}
		else
		{
			CE_API_ERROR("Could not get script fields");
			return ScriptFieldMap();
		}
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("CatEngineJITRuntime");
		if (rootDomain)
			s_ScriptData->RootDomain = rootDomain;
		else
			CE_API_ERROR("Could not load Jit!");

	}
	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_ScriptData->AppDomain);
		s_ScriptData->AppDomain = nullptr;
		
		mono_jit_cleanup(s_ScriptData->RootDomain);
		s_ScriptData->RootDomain = nullptr;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_ScriptData->EntityClasses.clear();
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_ScriptData->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_ScriptData->CoreAssemblyImage, "CatEngine", "MeownoBehaviour");
		

		for (int32_t i = 0; i < numTypes; i++)
		{

			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);


			const char* nameSpace = mono_metadata_string_heap(s_ScriptData->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_ScriptData->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;
			
			MonoClass* monoClass = mono_class_from_name(s_ScriptData->AppAssemblyImage, nameSpace, name);
			
			if (monoClass == entityClass)
				continue;

			bool isEntity = false;
			if (monoClass && entityClass)
				isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);

			if (!isEntity)
				continue;

			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, name);

			s_ScriptData->EntityClasses[fullName] = scriptClass;

			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t fieldFlag = mono_field_get_flags(field);

				if (fieldFlag == METHOD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);

					scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field };
				}
				
			}

		}


	}

	MonoImage* ScriptEngine::GetCoreImage()
	{
		return s_ScriptData->CoreAssemblyImage;
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

	ScriptClass::ScriptClass(const char* classNamespace, const char* className, bool isCore)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(isCore ? s_ScriptData->CoreAssemblyImage : s_ScriptData->AppAssemblyImage, classNamespace, className);

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

		if (m_Constructor)
		{
			UUID entityID = entity.GetUUID();
			void* params = &entityID;
			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &params);
		}
	}

	void ScriptInstance::InvokeUpdateMethod(float ts)
	{
		if (m_UpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_UpdateMethod, &param);
		}
	}

	void ScriptInstance::InvokeStartMethod()
	{
		if (m_StartMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_StartMethod);
	}
	bool ScriptInstance::GetFieldDataInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto& it = fields.find(name);

		if (it == fields.end())
			return false;
		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	void ScriptInstance::SetFieldDataInternal(const std::string& name, void* value)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto& it = fields.find(name);

		if (it == fields.end())
			return;
		const ScriptField& field = it->second;
		mono_field_set_value(m_Instance, field.ClassField, value);
	}
}
