#include <pch.h>

namespace ALEngine::Engine::Scene
{
	namespace
	{
		namespace rjs = rapidjson;
		using TWriter = rjs::PrettyWriter<rjs::StringBuffer>;
	}

	void WriteSprite(TWriter& writer, ECS::Entity en)
	{
		writer.Key("Sprite");
		writer.StartArray();
		writer.StartObject();

		Sprite const& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		// filePath
		writer.Key("filePath");
		writer.String( sprite.filePath.c_str(), static_cast<rjs::SizeType>( sprite.filePath.length() ) );

		// Colors
		writer.Key("color");
		writer.StartArray();
		writer.Double(static_cast<f64>(sprite.color.r));
		writer.Double(static_cast<f64>(sprite.color.g));
		writer.Double(static_cast<f64>(sprite.color.b));
		writer.Double(static_cast<f64>(sprite.color.a));
		writer.EndArray();

		// Layer
		writer.Key("layer");
		writer.Int64(static_cast<u64>(sprite.layer));

		writer.EndObject();
		writer.EndArray();
	}

	void ReadSprite(rjs::Value const& v, ECS::Entity en)
	{
		Sprite sprite;

		// Getting filePath
		sprite.filePath = v[0]["filePath"].GetString();

		// Getting colors
		rjs::Value const& c = v[0]["color"];
		sprite.color.r = c[0].GetFloat();
		sprite.color.g = c[1].GetFloat();
		sprite.color.b = c[2].GetFloat();
		sprite.color.a = c[3].GetFloat();

		// Getting layers
		sprite.layer = static_cast<RenderLayer>(v[0]["layer"].GetUint64());

		// Initialising value
		sprite.id = AssetManager::Instance()->GetGuid(sprite.filePath);
		sprite.index = 0;

		Coordinator::Instance()->AddComponent(en, sprite);
	}

	void WriteAnimator(TWriter& writer, ECS::Entity en)
	{
		writer.Key("Animator");
		writer.StartArray();
		writer.StartObject();

		Animator const& animator = Coordinator::Instance()->GetComponent<Animator>(en);
		// Saving animatorName
		writer.Key("animatorName");
		writer.String( animator.animatorName.c_str(), static_cast<rjs::SizeType>(animator.animatorName.length() ) );

		writer.EndObject();
		writer.EndArray();
	}

	void ReadAnimator(rjs::Value const& v, ECS::Entity en)
	{
		// Getting animatorName
		c8 const * animatorName = v[0]["animatorName"].GetString();
		Animator animator = ECS::CreateAnimator(animatorName);
		ECS::AttachAnimator(en, animator);
	}

	void WriteTransform(TWriter& writer, ECS::Entity en)
	{
		writer.Key("Transform");
		writer.StartArray();
		writer.StartObject();

		Transform const& transform = Coordinator::Instance()->GetComponent<Transform>(en);

		// Position
		writer.Key("position");
		writer.StartArray();
		writer.Double(static_cast<f64>(transform.position.x));
		writer.Double(static_cast<f64>(transform.position.y));
		writer.Double(static_cast<f64>(transform.position.z));
		writer.EndArray();

		// Scale
		writer.Key("scale");
		writer.StartArray();
		writer.Double(static_cast<f64>(transform.scale.x));
		writer.Double(static_cast<f64>(transform.scale.y));
		writer.EndArray();

		// Rotation
		writer.Key("rotation");
		writer.Double(static_cast<f64>(transform.rotation));

		writer.EndObject();
		writer.EndArray();
	}

	void ReadTransform(rjs::Value const& v, ECS::Entity en)
	{
		Transform transform;

		// Getting position
		rjs::Value const& p  = v[0]["position"];
		transform.position.x = p[0].GetFloat();
		transform.position.y = p[1].GetFloat();
		transform.position.z = p[2].GetFloat();

		// Getting scale
		rjs::Value const& s = v[0]["scale"];
		transform.scale.x = s[0].GetFloat();
		transform.scale.y = s[1].GetFloat();

		// Getting rotation
		transform.rotation = v[0]["rotation"].GetFloat();

		Coordinator::Instance()->AddComponent(en, transform);
	}

	void WriteEntityData(TWriter& writer, ECS::Entity en)
	{
		writer.Key("EntityData");
		writer.StartArray();
		writer.StartObject();

		EntityData const& entityData = Coordinator::Instance()->GetComponent<EntityData>(en);

		// Tag
		writer.Key("tag");
		writer.String( entityData.tag.c_str(), static_cast<rjs::SizeType>(entityData.tag.length() ) );

		// Active status
		writer.Key("active");
		writer.Bool(entityData.active);

		// Entity ID
		writer.Key("id");
		writer.Uint(entityData.id);

		writer.EndObject();
		writer.EndArray();
	}

	void ReadEntityData(rjs::Value const& v, ECS::Entity en)
	{

	}

	void WriteCollider2D(TWriter& writer, ECS::Entity en)
	{

	}

	void ReadCollider2D(rjs::Value const& v, ECS::Entity en)
	{

	}

	void WriteRigidbody2D(TWriter& writer, ECS::Entity en)
	{

	}

	void ReadRigidbody2D(rjs::Value const& v, ECS::Entity en)
	{

	}

	void WriteCharacterController(TWriter& writer, ECS::Entity en)
	{

	}

	void ReadCharacterController(rjs::Value const& v, ECS::Entity en)
	{

	}

	void WriteEventTrigger(TWriter& writer, ECS::Entity en)
	{

	}

	void ReadEventTrigger(rjs::Value const& v, ECS::Entity en)
	{

	}

	void WriteEventCollisionTrigger(TWriter& writer, ECS::Entity en)
	{

	}

	void ReadEventCollisionTrigger(rjs::Value const& v, ECS::Entity en)
	{

	}

	void WriteUnit(TWriter& writer, ECS::Entity en)
	{

	}

	void ReadUnit(rjs::Value const& v, ECS::Entity en)
	{

	}

	void WriteCell(TWriter& writer, ECS::Entity en)
	{

	}

	void ReadCell(rjs::Value const& v, ECS::Entity en)
	{

	}

	void SaveScene(c8 const* sceneName)
	{
		std::string const& filePath = "Assets\\" + std::string(sceneName) + ".scene";
		rjs::StringBuffer sb{};
		TWriter writer(sb);

		ECS::EntityList const& entities = Coordinator::Instance()->GetEntities();
		u32 id{ 0 };
		writer.StartArray();
		for (auto it{ entities.begin() }; it != entities.end(); ++it)
		{
			writer.StartObject();

			writer.String("id");
			writer.Uint(id++);

			ECS::Entity en = *it;
			if (Coordinator::Instance()->HasComponent<Sprite>(en))
				WriteSprite(writer, en);
			if (Coordinator::Instance()->HasComponent<Animator>(en))
				WriteAnimator(writer, en);
			if (Coordinator::Instance()->HasComponent<Transform>(en))
				WriteTransform(writer, en);
			if (Coordinator::Instance()->HasComponent<EntityData>(en))
				WriteEntityData(writer, en);
			if (Coordinator::Instance()->HasComponent<Collider2D>(en))
				WriteCollider2D(writer, en);
			if (Coordinator::Instance()->HasComponent<Rigidbody2D>(en))
				WriteRigidbody2D(writer, en);
			if (Coordinator::Instance()->HasComponent<CharacterController>(en))
				WriteCharacterController(writer, en);
			if (Coordinator::Instance()->HasComponent<EventTrigger>(en))
				WriteEventTrigger(writer, en);
			if (Coordinator::Instance()->HasComponent<EventCollisionTrigger>(en))
				WriteEventCollisionTrigger(writer, en);
			if (Coordinator::Instance()->HasComponent<Unit>(en))
				WriteUnit(writer, en);
			if (Coordinator::Instance()->HasComponent<Cell>(en))
				WriteCell(writer, en);

			writer.EndObject();
		}
		writer.EndArray();

		// save into a file
		std::ofstream ofs{ filePath };
		if (!ofs)
		{
			AL_CORE_WARN("Unable to save into file!");
			return;
		}
		ofs.write(sb.GetString(), sb.GetLength());
	}

	void LoadScene(c8 const* sceneName)
	{
		std::ifstream ifs{ sceneName, std::ios::ate };
		if (!ifs)
		{
			AL_CORE_WARN("Unable to open scene: {}", sceneName);
			return;
		}
		u64 const size = ifs.tellg();
		ifs.seekg(ifs.beg);
		c8* buffer = Memory::DynamicMemory::New<c8>(size);
		ifs.read(buffer, size);

		rjs::Document doc;
		doc.Parse(buffer);
		Memory::DynamicMemory::Delete(buffer);

		for (rjs::Value::ValueIterator it{ doc.Begin() }; it != doc.End(); ++it)
		{
			ECS::Entity en = Coordinator::Instance()->CreateEntity();
			rjs::Value const& v{ *it };
			if (v.HasMember("Sprite"))
				ReadSprite(v["Sprite"], en);
			if (v.HasMember("Animator"))
				ReadAnimator(v["Animator"], en);
			if (v.HasMember("Transform"))
				ReadTransform(v["Transform"], en);
			if (v.HasMember("EntityData"))
				ReadEntityData(v["EntityData"], en);
			if (v.HasMember("Collider2D"))
				ReadCollider2D(v["Collider2D"], en);
			if (v.HasMember("Rigidbody2D"))
				ReadRigidbody2D(v["Rigidbody2D"], en);
			if (v.HasMember("CharacterController"))
				ReadCharacterController(v["CharacterController"], en);
			if (v.HasMember("EventTrigger"))
				ReadEventTrigger(v["EventTrigger"], en);
			if (v.HasMember("EventCollisionTrigger"))
				ReadEventCollisionTrigger(v["EventCollisionTrigger"], en);
			if (v.HasMember("Unit"))
				ReadUnit(v["Unit"], en);
			if (v.HasMember("Cell"))
				ReadCell(v["Cell"], en);
		}
	}
}