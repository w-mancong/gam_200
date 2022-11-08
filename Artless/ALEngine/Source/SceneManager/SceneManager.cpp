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
		Sprite sprite{};

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
		Transform transform{};

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
		EntityData& entityData = Coordinator::Instance()->GetComponent<EntityData>(en);

		// Getting tag
		entityData.tag = v[0]["tag"].GetString();
		// Getting active status
		entityData.active = v[0]["active"].GetBool();
		// Getting id
		entityData.id = v[0]["id"].GetUint();
	}

	void WriteCollider2D(TWriter& writer, ECS::Entity en)
	{
		writer.Key("Collider2D");
		writer.StartArray();
		writer.StartObject();

		Collider2D const& collider = Coordinator::Instance()->GetComponent<Collider2D>(en);
		// Collider's type
		writer.Key("colliderType");
		writer.Uint64(static_cast<u64>(collider.colliderType));

		// Rotation
		writer.Key("rotation");
		writer.Double(static_cast<f64>(collider.rotation));

		// Scale
		writer.Key("scale");
		writer.StartArray();
		writer.Double(static_cast<f64>(collider.scale[0]));
		writer.Double(static_cast<f64>(collider.scale[1]));
		writer.EndArray();

		// IsCollided
		writer.Key("isCollided");
		writer.Bool(collider.isCollided);

		// IsTrigger
		writer.Key("isTrigger");
		writer.Bool(collider.isTrigger);

		// IsDebug
		writer.Key("isDebug");
		writer.Bool(collider.isDebug);

		// IsEnabled
		writer.Key("isEnabled");
		writer.Bool(collider.isEnabled);

		// Local position
		writer.Key("m_localPosition");
		writer.StartArray();
		writer.Double(static_cast<f64>(collider.m_localPosition.x));
		writer.Double(static_cast<f64>(collider.m_localPosition.y));
		writer.EndArray();

		writer.EndObject();
		writer.EndArray();
	}

	void ReadCollider2D(rjs::Value const& v, ECS::Entity en)
	{
		Collider2D collider{};

		// Getting Collider Type
		collider.colliderType = static_cast<ColliderType>(v[0]["colliderType"].GetUint64());

		// Getting rotation
		collider.rotation = v[0]["rotation"].GetFloat();

		// Getting scale
		rjs::Value const& s = v[0]["scale"];
		collider.scale[0] = s[0].GetFloat();
		collider.scale[1] = s[1].GetFloat();

		// Getting IsCollided
		collider.isCollided = v[0]["isCollided"].GetBool();

		// Getting IsTrigger
		collider.isTrigger = v[0]["isTrigger"].GetBool();

		// Getting IsDebug
		collider.isDebug = v[0]["isDebug"].GetBool();

		// Getting IsEnabled
		collider.isEnabled = v[0]["isEnabled"].GetBool();

		// Getting local position
		rjs::Value const& p = v[0]["m_localPosition"];
		collider.m_localPosition.x = p[0].GetFloat();
		collider.m_localPosition.y = p[1].GetFloat();

		Coordinator::Instance()->AddComponent(en, collider);
	}

	void WriteRigidbody2D(TWriter& writer, ECS::Entity en)
	{
		writer.Key("Rigidbody2D");
		writer.StartArray();
		writer.StartObject();

		Rigidbody2D const& rb = Coordinator::Instance()->GetComponent<Rigidbody2D>(en);

		// Drag
		writer.Key("drag");
		writer.StartArray();
		writer.Double(static_cast<f64>(rb.drag.x));
		writer.Double(static_cast<f64>(rb.drag.y));
		writer.EndArray();

		// Mass
		writer.Key("mass");
		writer.Double(static_cast<f64>(rb.mass));

		// Has Gravity
		writer.Key("hasGravity");
		writer.Bool(rb.hasGravity);

		// Is enabled
		writer.Key("isEnabled");
		writer.Bool(rb.isEnabled);

		writer.EndObject();
		writer.EndArray();
	}

	void ReadRigidbody2D(rjs::Value const& v, ECS::Entity en)
	{
		Rigidbody2D rb{};

		// Getting drag
		rjs::Value const& d = v[0]["drag"];
		rb.drag.x = d[0].GetFloat();
		rb.drag.y = d[1].GetFloat();

		// Getting mass
		rb.mass = v[0]["mass"].GetFloat();

		// Getting hasGravity
		rb.hasGravity = v[0]["hasGravity"].GetBool();

		// Getting is enabled
		rb.isEnabled = v[0]["isEnabled"].GetBool();
	}

	void WriteCharacterController(TWriter& writer, ECS::Entity en)
	{
		writer.Key("CharacterController");
		writer.StartArray();
		writer.StartObject();

		CharacterController const& cc = Coordinator::Instance()->GetComponent<CharacterController>(en);
		
		// Speed
		writer.Key("speed");
		writer.Double(static_cast<f64>(cc.speed));

		// Jump Strength
		writer.Key("jumpStrength");
		writer.Double(static_cast<f64>(cc.jumpStrength));

		// Left key
		writer.Key("leftKey");
		writer.Uint64(cc.leftKey);

		// Right key
		writer.Key("rightKey");
		writer.Uint64(cc.rightKey);

		// Jumpy key
		writer.Key("jumpKey");
		writer.Uint64(cc.jumpKey);

		writer.EndObject();
		writer.EndArray();
	}

	void ReadCharacterController(rjs::Value const& v, ECS::Entity en)
	{
		CharacterController cc{};

		// Getting Speed
		cc.speed = v[0]["speed"].GetFloat();

		// Getting jump strength
		cc.jumpStrength = v[0]["jumpStrength"].GetFloat();

		// Getting left key
		cc.leftKey = v[0]["leftKey"].GetUint64();

		// Getting right key
		cc.rightKey = v[0]["rightKey"].GetUint64();

		// Getting jump key
		cc.jumpKey = v[0]["jumpKey"].GetUint64();
	}

	void WriteEventTrigger(TWriter& writer, ECS::Entity en)
	{
		/*
			Event Trigger

			OnPointerEnter
			OnPointerStay
			OnPointerExit
			OnPointerClick
		*/
		writer.Key("EventTrigger");
		writer.StartArray();
		writer.StartObject();

		EventTrigger const& et = Coordinator::Instance()->GetComponent<EventTrigger>(en);

		// OnPointerEnter	

		writer.EndObject();
		writer.EndArray();
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