#include "cepch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "CatEngine/Components/Components.h"
#include "CatEngine/Scripting/ScriptEngine.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>


namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}


namespace CatEngine
{






	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const entt::entity& id)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << (uint64_t)(uint32_t)id << YAML::EndSeq;
		return out;
	}

	static std::string GetNamedComponents(YAML::Node& node, std::string component, std::string targetItem)
	{
		std::string entityComponent;
		auto entityNode = node[component];
		if (entityNode)
			entityComponent = entityNode[targetItem].as<std::string>();
		return entityComponent = "" ? std::string() : entityComponent;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}


	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		CE_PROFILE_FUNCTION();

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<IDComponent>().ID;
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}
		if (entity.HasComponent<LayerComponent>())
		{
			out << YAML::Key << "LayerComponent";
			out << YAML::BeginMap; // LayerComponent

			auto& layer = entity.GetComponent<LayerComponent>().Layer;
			out << YAML::Key << "Layer" << YAML::Value << layer;

			out << YAML::EndMap; // LayerComponent
		}
		if (entity.HasComponent<NameComponent>())
		{
			out << YAML::Key << "NameComponent";
			out << YAML::BeginMap; // NameComponent

			auto& name = entity.GetComponent<NameComponent>().Name;
			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap; // NameComponent
		}
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& src = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << src.Color;
			std::string texturePath = src.Texture != nullptr ? src.Texture->GetFilePath() : "";
			out << YAML::Key << "Texture" << YAML::Value << texturePath;
			out << YAML::Key << "Tiling" << YAML::Value << src.TilingFactor;
			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap; // CircleRendererComponent

			auto& src = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << src.Color;
			out << YAML::Key << "Thickness" << YAML::Value << src.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << src.Fade;
			out << YAML::EndMap; // CircleRendererComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFov();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap; // Rigidbody2DComponent
			out << YAML::Key << "BodyType" << YAML::Value << (int)rb2d.Type;
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;
			out << YAML::EndMap; // Rigidbody2DComponent

		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "BoxCollider2DComponent" << YAML::Value;
			out << YAML::BeginMap; // BoxCollider2DComponent
			out << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2d.Size;
			out << YAML::Key << "Rotation" << YAML::Value << bc2d.Rotation;
			out << YAML::Key << "ShowColliderBounds" << YAML::Value << bc2d.ShowColliderBounds;
			out << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2d.RestitutionThreshold;
			out << YAML::EndMap; // BoxCollider2DComponent

		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "CircleCollider2DComponent" << YAML::Value;
			out << YAML::BeginMap; // CircleCollider2DComponent
			out << YAML::Key << "Offset" << YAML::Value << cc2d.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2d.Radius;
			out << YAML::Key << "ShowColliderBounds" << YAML::Value << cc2d.ShowColliderBounds;
			out << YAML::Key << "Density" << YAML::Value << cc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2d.RestitutionThreshold;
			out << YAML::EndMap; // CircleCollider2DComponent

		}

		if (entity.HasComponent<ScriptComponent>())
		{
			auto& sc = entity.GetComponent<ScriptComponent>();
			out << YAML::Key << "ScriptComponent" << YAML::Value;
			out << YAML::BeginMap; // ScriptComponent
			out << YAML::Key << "ClassName" << YAML::Value << sc.ClassName;

			// Fields
			Ref<ScriptClass> scriptClass = ScriptEngine::GetScriptClass(sc.ClassName);

			if (scriptClass)
			{
				const auto& fields = scriptClass->GetFields();
				if (fields.size() > 0)
				{
					out << YAML::Key << "ScriptFields" << YAML::Value;
					auto& scriptFields = ScriptEngine::GetScriptFieldMap(entity);
					out << YAML::BeginSeq;
					for (const auto& [name, field] : fields)
					{
						if (scriptFields.find(name) == scriptFields.end())
							continue;

						// - Name : FieldName
						//   Type : 1
						//   Data : 6.0f

						out << YAML::BeginMap; // ScriptFields

						out << YAML::Key << "Name" << YAML::Value << name;
						out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);
						out << YAML::Key << "Data" << YAML::Value;

						ScriptFieldInstance& scriptFieldInstance = scriptFields.at(name);

#define FIELD_TYPE(FieldType, Type) case ScriptFieldType::FieldType:\
										out << scriptFieldInstance.GetValue<Type>();\
										break;

						switch (field.Type)
						{
							FIELD_TYPE(Float, float);
							FIELD_TYPE(Double, double);
							FIELD_TYPE(SByte, int8_t);
							FIELD_TYPE(Char, char);
							FIELD_TYPE(Int16, int16_t);
							FIELD_TYPE(Int32, int32_t);
							FIELD_TYPE(Int64, int64_t);
							FIELD_TYPE(Boolean, bool);
							FIELD_TYPE(Byte, uint8_t);
							FIELD_TYPE(UInt16, uint16_t);
							FIELD_TYPE(UInt32, uint32_t);
							FIELD_TYPE(UInt64, uint64_t);
							FIELD_TYPE(String, char*);
							FIELD_TYPE(Vector2, glm::vec2);
							FIELD_TYPE(Vector3, glm::vec3);
							FIELD_TYPE(Vector4, glm::vec4);
							FIELD_TYPE(Object, uint64_t);
						}
#undef FIELD_TYPE

						out << YAML::EndMap; // ScriptFields
					}
					out << YAML::EndSeq;
				}
			}
			out << YAML::EndMap; // ScriptComponent

		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		CE_PROFILE_FUNCTION();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "SampleScene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (auto entityID : m_Scene->m_Registry.view<entt::entity>())
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}
	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		CE_PROFILE_FUNCTION();

		CE_API_ASSERT(false, "Not Implemented Yet");
	}
	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		CE_PROFILE_FUNCTION();

		YAML::Node data;
		try	{ data = YAML::LoadFile(filepath); }
		catch (YAML::ParserException ex)
		{
			CE_API_ERROR("Failed to load .catengine file '{0}'\n       {1}", filepath, ex.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto nameComponent = entity["NameComponent"];
				if (nameComponent)
					name = nameComponent["Name"].as<std::string>();

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				std::string tag;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					tag = tagComponent["Tag"].as<std::string>();
				deserializedEntity.GetComponent<TagComponent>().Tag = tag;

				std::string layer;
				auto layerComponent = entity["LayerComponent"];
				if (layerComponent)
					layer = layerComponent["Layer"].as<std::string>();
				deserializedEntity.GetComponent<LayerComponent>().Layer = layer;

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetOrAddComponent<TransformComponent>();
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.GetOrAddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					if (spriteRendererComponent["Texture"].as<std::string>() != "")
						src.Texture = Texture2D::Create(spriteRendererComponent["Texture"].as<std::string>());
					else
						src.Texture = nullptr;
					src.TilingFactor = spriteRendererComponent["Tiling"].as<float>();
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& src = deserializedEntity.GetOrAddComponent<CircleRendererComponent>();
					src.Color = circleRendererComponent["Color"].as<glm::vec4>();
					src.Thickness = circleRendererComponent["Thickness"].as<float>();
					src.Fade = circleRendererComponent["Fade"].as<float>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFov(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto rigidBody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidBody2DComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb2d.Type = (Rigidbody2DComponent::BodyType)rigidBody2DComponent["BodyType"].as<int>();
					rb2d.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2D = entity["BoxCollider2DComponent"];
				if (boxCollider2D)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();

					bc2d.Offset = boxCollider2D["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2D["Size"].as<glm::vec2>();
					bc2d.Rotation = boxCollider2D["Rotation"].as<float>();
					bc2d.ShowColliderBounds = boxCollider2D["ShowColliderBounds"].as<bool>();
					bc2d.Density = boxCollider2D["Density"].as<float>();
					bc2d.Friction = boxCollider2D["Friction"].as<float>();
					bc2d.Restitution = boxCollider2D["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2D["RestitutionThreshold"].as<float>();

				}

				auto circleCollider2D = entity["CircleCollider2DComponent"];
				if (circleCollider2D)
				{
					auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();

					cc2d.Offset = circleCollider2D["Offset"].as<glm::vec2>();
					cc2d.Radius = circleCollider2D["Radius"].as<float>();
					cc2d.ShowColliderBounds = circleCollider2D["ShowColliderBounds"].as<bool>();
					cc2d.Density = circleCollider2D["Density"].as<float>();
					cc2d.Friction = circleCollider2D["Friction"].as<float>();
					cc2d.Restitution = circleCollider2D["Restitution"].as<float>();
					cc2d.RestitutionThreshold = circleCollider2D["RestitutionThreshold"].as<float>();

				}
				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();

					sc.ClassName = scriptComponent["ClassName"].as<std::string>();
					
					auto scriptFields = scriptComponent["ScriptFields"];

					if (scriptFields)
					{
						Ref<ScriptClass> scriptClass = ScriptEngine::GetScriptClass(sc.ClassName);
						if (!scriptClass)
							continue;
						const auto& fields = scriptClass->GetFields();
						auto& scriptFieldMap = ScriptEngine::GetScriptFieldMap(deserializedEntity);

						for (auto scriptField : scriptFields)
						{
							std::string fieldName = scriptField["Name"].as<std::string>();
							std::string typeString = scriptField["Type"].as<std::string>();
							ScriptFieldType type = Utils::StringToScriptFieldType(typeString);

							ScriptFieldInstance& fieldInstance = scriptFieldMap[fieldName];
							if (fields.find(fieldName) == fields.end())
								continue;
							fieldInstance.Field = fields.at(fieldName);

#define FIELD_TYPE(Type)  Type data = scriptField["Data"].as<Type>(); \
							fieldInstance.SetValue(data); \
							break;
									

							switch (type)
							{
								case ScriptFieldType::Float:
								{
									FIELD_TYPE(float);
								}
								case ScriptFieldType::Double:
								{
									FIELD_TYPE(double);
								}
								case ScriptFieldType::SByte:
								{
									FIELD_TYPE(int8_t);
								}
								case ScriptFieldType::Char:
								{
									FIELD_TYPE(char);
								}
								case ScriptFieldType::Int16:
								{
									FIELD_TYPE(int16_t);
								}
								case ScriptFieldType::Int32:
								{
									FIELD_TYPE(int32_t);
								}
								case ScriptFieldType::Int64:
								{
									FIELD_TYPE(int64_t);
								}
								case ScriptFieldType::Boolean:
								{
									FIELD_TYPE(bool);
								}
								case ScriptFieldType::Byte:
								{
									FIELD_TYPE(uint16_t);
								}
								case ScriptFieldType::UInt16:
								{
									FIELD_TYPE(uint16_t);
								}
								case ScriptFieldType::UInt32:
								{
									FIELD_TYPE(uint32_t);
								}
								case ScriptFieldType::UInt64:
								{
									FIELD_TYPE(uint64_t);
								}
								case ScriptFieldType::String:
								{
									FIELD_TYPE(std::string);
								}
								case ScriptFieldType::Vector2:
								{
									FIELD_TYPE(glm::vec2);
								}
								case ScriptFieldType::Vector3:
								{
									FIELD_TYPE(glm::vec3);
								}
								case ScriptFieldType::Vector4:
								{
									FIELD_TYPE(glm::vec4);
								}
								case ScriptFieldType::Object:
								{
									FIELD_TYPE(uint64_t);
								}
							}
						}
					}
				}
			}
		}
		return true;
	}
	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		CE_PROFILE_FUNCTION();

		CE_API_ASSERT(false, "Not Implemented Yet");
		return false;
	}
}
