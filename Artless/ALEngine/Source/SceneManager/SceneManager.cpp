#include <pch.h>

namespace ALEngine::Engine::Scene
{
	using TWriter = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

	void WriteSprite(TWriter& writer, ECS::Entity en)
	{
		writer.Key("Sprite");
		writer.StartArray();
		writer.StartObject();

		Sprite const& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		// filePath
		writer.Key("filePath");
		writer.String( sprite.filePath.c_str(), static_cast<rapidjson::SizeType>( sprite.filePath.length() ) );

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

	void WriteAnimator(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteTransform(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteEntityData(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteCollider2D(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteRigidbody2D(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteCharacterController(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteEventTrigger(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteEventCollisionTrigger(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteUnit(TWriter& writer, ECS::Entity en)
	{

	}

	void WriteCell(TWriter& writer, ECS::Entity en)
	{

	}

	void SaveScene(c8 const* sceneName)
	{
		std::string const& filePath = "Assets\\" + std::string(sceneName) + ".scene";
		rapidjson::StringBuffer sb{};
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
}