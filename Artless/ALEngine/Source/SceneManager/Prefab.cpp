/*!
file:	Prefabs.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declarations for saving, instantiating and clearing
		the memory of all prefrabs instantiated previously

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <SceneManager/Prefabs.h>

namespace ALEngine
{
	namespace
	{
		namespace rjs = rapidjson;
		using TWriter = rjs::PrettyWriter<rjs::StringBuffer>;

		ECS::Entity constexpr MAX_PREFABS = 1'000;
		
		//std::queue<ECS::Entity> prefabs;
		std::unordered_map<std::string, std::string> instObjects;
	}

	void WriteSprite(TWriter& writer, ECS::Entity en)
	{
		writer.Key("Sprite");
		writer.StartArray();
		writer.StartObject();

		Sprite const& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		// filePath
		writer.Key("filePath");
		writer.String(sprite.filePath.c_str(), static_cast<rjs::SizeType>(sprite.filePath.length()));

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
		writer.Uint64(static_cast<u64>(sprite.layer));

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
		//sprite.layer = static_cast<RenderLayer>(v[0]["layer"].GetUint64());
		sprite.layer = v[0]["layer"].GetUint();

		// Initialising value
		sprite.id = Engine::AssetManager::Instance()->GetGuid(sprite.filePath);
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
		writer.String(animator.animatorName.c_str(), static_cast<rjs::SizeType>(animator.animatorName.length()));

		writer.EndObject();
		writer.EndArray();
	}

	void ReadAnimator(rjs::Value const& v, ECS::Entity en)
	{
		// Getting animatorName
		c8 const* animatorName = v[0]["animatorName"].GetString();
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

		// UI
		writer.Key("ui");
		writer.Bool(transform.ui);

		// uiOffset
		writer.Key("uiOffset");
		writer.StartArray();
		writer.Double(static_cast<f64>(transform.uiOffset.x));
		writer.Double(static_cast<f64>(transform.uiOffset.y));
		writer.Double(static_cast<f64>(transform.uiOffset.z));
		writer.EndArray();

		writer.EndObject();
		writer.EndArray();
	}

	void ReadTransform(rjs::Value const& v, ECS::Entity en)
	{
		Transform transform{};

		// Getting position
		rjs::Value const& p = v[0]["position"];
		transform.position.x = p[0].GetFloat();
		transform.position.y = p[1].GetFloat();
		transform.position.z = p[2].GetFloat();

		// Getting scale
		rjs::Value const& s = v[0]["scale"];
		transform.scale.x = s[0].GetFloat();
		transform.scale.y = s[1].GetFloat();

		// Getting rotation
		transform.rotation = v[0]["rotation"].GetFloat();

		// Getting UI status
		transform.ui = v[0]["ui"].GetBool();

		// Getting uiOffset
		rjs::Value const& t = v[0]["uiOffset"];
		transform.uiOffset.x = t[0].GetFloat();
		transform.uiOffset.y = t[1].GetFloat();
		transform.uiOffset.z = t[2].GetFloat();

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
		writer.String(entityData.tag.c_str(), static_cast<rjs::SizeType>(entityData.tag.length()));

		// Active status
		writer.Key("active");
		writer.Bool(entityData.active);

		// Local active status
		writer.Key("localActive");
		writer.Bool(entityData.selfActive);

		// Entity ID
		writer.Key("id");
		writer.Uint(entityData.id);

		// Entity's parent id for scene graph
		writer.Key("parentID");
		writer.Int(entityData.parentID);

#if EDITOR
		// Tree Node flag for imgui
		writer.Key("treeNodeFlags");
		writer.Uint64(static_cast<u64>(entityData.treeNodeFlags));
#endif

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
		// Getting local active status
		entityData.selfActive = v[0]["localActive"].GetBool();
		// Getting id
		entityData.id = v[0]["id"].GetUint();
		// Getting parent id
		entityData.parentID = v[0]["parentID"].GetInt();

#if EDITOR
		// Getting tree node flags for imgui
		entityData.treeNodeFlags = static_cast<ImGuiTreeNodeFlags_>(v[0]["treeNodeFlags"].GetUint64());
#endif
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

		Coordinator::Instance()->AddComponent(en, rb);
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

		Coordinator::Instance()->AddComponent(en, cc);
	}

	void WriteUnit(TWriter& writer, ECS::Entity en)
	{
		writer.Key("Unit");
		writer.StartArray();
		writer.StartObject();

		Unit const& unit = Coordinator::Instance()->GetComponent<Unit>(en);

		// Max health
		writer.Key("maxHealth");
		writer.Int(unit.maxHealth);

		// Max movement points
		writer.Key("maxMovementPoints");
		writer.Int(unit.maxActionPoints);

		// Unit type
		writer.Key("unitType");
		writer.Uint64(static_cast<u64>(unit.unitType));

		writer.EndObject();
		writer.EndArray();
	}

	void ReadUnit(rjs::Value const& v, ECS::Entity en)
	{
		Unit unit{};

		// Getting max health
		unit.maxHealth = v[0]["maxHealth"].GetInt();

		// Getting max movement points
		unit.maxActionPoints = v[0]["maxMovementPoints"].GetInt();

		// Getting unit type
		unit.unitType = static_cast<UNIT_TYPE>(v[0]["unitType"].GetUint64());

		Coordinator::Instance()->AddComponent(en, unit);
	}

	void WriteParticleProperty(TWriter& writer, ECS::Entity en)
	{
		writer.Key("ParticleProperty");
		writer.StartArray();
		writer.StartObject();

		ParticleProperties const& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);

		// velocity
		writer.Key("velocity");
		writer.StartArray();
		writer.Double(static_cast<f64>(prop.velocity.x));
		writer.Double(static_cast<f64>(prop.velocity.y));
		writer.EndArray();

		// velocityVariation
		writer.Key("velocityVariation");
		writer.StartArray();
		writer.Double(static_cast<f64>(prop.velocityVariation.x));
		writer.Double(static_cast<f64>(prop.velocityVariation.y));
		writer.EndArray();

		// colorStart
		writer.Key("colorStart");
		writer.StartArray();
		writer.Double(static_cast<f64>(prop.colorStart.x));
		writer.Double(static_cast<f64>(prop.colorStart.y));
		writer.Double(static_cast<f64>(prop.colorStart.z));
		writer.Double(static_cast<f64>(prop.colorStart.w));
		writer.EndArray();

		// colorEnd
		writer.Key("colorEnd");
		writer.StartArray();
		writer.Double(static_cast<f64>(prop.colorEnd.x));
		writer.Double(static_cast<f64>(prop.colorEnd.y));
		writer.Double(static_cast<f64>(prop.colorEnd.z));
		writer.Double(static_cast<f64>(prop.colorEnd.w));
		writer.EndArray();

		// sizeStart
		writer.Key("sizeStart");
		writer.StartArray();
		writer.Double(static_cast<f64>(prop.sizeStart.x));
		writer.Double(static_cast<f64>(prop.sizeStart.y));
		writer.EndArray();

		// sizeEnd
		writer.Key("sizeEnd");
		writer.StartArray();
		writer.Double(static_cast<f64>(prop.sizeEnd.x));
		writer.Double(static_cast<f64>(prop.sizeEnd.y));
		writer.EndArray();

		// sizeVariation
		writer.Key("sizeVariation");
		writer.Double(static_cast<f64>(prop.sizeVariation));

		// lifeTime
		writer.Key("lifeTime");
		writer.Double(static_cast<f64>(prop.lifeTime));

		// spawnRate
		writer.Key("spawnRate");
		writer.Double(static_cast<f64>(prop.spawnRate));

		// rotation
		writer.Key("rotation");
		writer.Double(static_cast<f64>(prop.rotation));

		// gravityEnabled
		writer.Key("gravityEnabled");
		writer.Bool(prop.gravityEnabled);

		// active
		writer.Key("active");
		writer.Bool(prop.active);

		writer.EndObject();
		writer.EndArray();
	}

	void ReadParticleProperty(rjs::Value const& v, ECS::Entity en)
	{
		ParticleProperties prop{};

		// Getting velocity
		rjs::Value const& p = v[0]["velocity"];
		prop.velocity.x = p[0].GetFloat();
		prop.velocity.y = p[1].GetFloat();

		// Getting velocityVariation
		rjs::Value const& q = v[0]["velocityVariation"];
		prop.velocityVariation.x = q[0].GetFloat();
		prop.velocityVariation.y = q[1].GetFloat();

		// Getting colorStart
		rjs::Value const& w = v[0]["colorStart"];
		prop.colorStart.x = w[0].GetFloat();
		prop.colorStart.y = w[1].GetFloat();
		prop.colorStart.z = w[2].GetFloat();
		prop.colorStart.w = w[3].GetFloat();

		// Getting colorEnd
		rjs::Value const& e = v[0]["colorEnd"];
		prop.colorEnd.x = e[0].GetFloat();
		prop.colorEnd.y = e[1].GetFloat();
		prop.colorEnd.z = e[2].GetFloat();
		prop.colorEnd.w = e[3].GetFloat();

		// Getting sizeStart
		rjs::Value const& a = v[0]["sizeStart"];
		prop.sizeStart.x = a[0].GetFloat();
		prop.sizeStart.y = a[1].GetFloat();

		// Getting sizeEnd
		rjs::Value const& b = v[0]["sizeEnd"];
		prop.sizeEnd.x = b[0].GetFloat();
		prop.sizeEnd.y = b[1].GetFloat();

		// Getting sizeVariation
		prop.sizeVariation = v[0]["sizeVariation"].GetFloat();

		// Getting lifetime
		prop.lifeTime = v[0]["lifeTime"].GetFloat();

		// Getting spawnRate
		prop.spawnRate = v[0]["spawnRate"].GetFloat();

		// Getting rotation
		prop.rotation = v[0]["rotation"].GetFloat();

		// Getting gravityEnabled
		prop.gravityEnabled = v[0]["gravityEnabled"].GetBool();

		// Getting active
		prop.active = v[0]["active"].GetBool();

		Coordinator::Instance()->AddComponent(en, prop);
	}

	void WriteTextProperty(TWriter& writer, ECS::Entity en)
	{
		writer.Key("TextProperty");
		writer.StartArray();
		writer.StartObject();

		Text const& prop = Coordinator::Instance()->GetComponent<Text>(en);

		// velocity
		writer.Key("textString");
		writer.String(prop.textString.c_str());

		writer.Key("currentFont");
		writer.String(prop.currentFont.c_str());

		writer.Key("scale");
		writer.Double(static_cast<f64>(prop.scale));

		// color
		writer.Key("color");
		writer.StartArray();
		writer.Double(static_cast<f64>(prop.colour.x));
		writer.Double(static_cast<f64>(prop.colour.y));
		writer.Double(static_cast<f64>(prop.colour.z));
		writer.EndArray();

		// colorEnd
		writer.Key("position");
		writer.StartArray();
		writer.Double(static_cast<f64>(prop.position.x));
		writer.Double(static_cast<f64>(prop.position.y));
		writer.EndArray();

		writer.EndObject();
		writer.EndArray();
	}

	void ReadTextProperty(rjs::Value const& v, ECS::Entity en)
	{
		Text prop{};

		prop.textString = v[0]["textString"].GetString();

		prop.currentFont = v[0]["currentFont"].GetString();

		prop.scale = v[0]["scale"].GetFloat();

		// Getting color
		rjs::Value const& w = v[0]["color"];
		prop.colour.x = w[0].GetFloat();
		prop.colour.y = w[1].GetFloat();
		prop.colour.z = w[2].GetFloat();

		// Getting position
		rjs::Value const& e = v[0]["position"];
		prop.position.x = e[0].GetFloat();
		prop.position.y = e[1].GetFloat();

		Coordinator::Instance()->AddComponent(en, prop);
	}

	void WriteLogicComponent(TWriter& writer, ECS::Entity en)
	{
		writer.Key("LogicComponent");
		writer.StartArray();
		writer.StartObject();

		LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);
		writer.Key("components");
		writer.StartArray();
		for (auto const& it : lc.logics)
		{
			std::string const& name = it.first;
			writer.String(name.c_str(), static_cast<rjs::SizeType>(name.length()));
		}
		writer.EndArray();

		writer.EndObject();
		writer.EndArray();
	}

	void ReadLogicComponent(rjs::Value const& v, ECS::Entity en)
	{
		// Getting the names of the components
		rjs::Value const& c = v[0]["components"];
		for (u64 i = 0; i < c.Size(); ++i)
		{
			c8 const* name = c[i].GetString();
			rttr::type class_type = rttr::type::get_by_name(name);
			rttr::variant var = class_type.create();
			class_type.invoke("DeserializeComponent", var, { en });
		}
	}

	void WriterAudioComponent(TWriter& writer, ECS::Entity en)
	{
		writer.Key("AudioSource");
		writer.StartArray();
		writer.StartObject();

		Engine::AudioSource const& as = Coordinator::Instance()->GetComponent<Engine::AudioSource>(en);
		writer.Key("audioClips");
		writer.StartArray();
		for (auto const& it : as.list)
		{
			Engine::Audio const& audio = it.second;
			writer.String( audio.m_AudioName.c_str(), static_cast<rjs::SizeType>( audio.m_AudioName.length() ) );	// 0
			writer.Double( audio.m_Volume );					// 1
			writer.Bool( audio.m_Loop );						// 2
			writer.Bool( audio.m_Mute );						// 3
			writer.Uint64( static_cast<u64>(audio.m_Channel) );	// 4
		}
		writer.EndArray();

		writer.EndObject();
		writer.EndArray();
	}

	void ReadAudioComponent(rjs::Value const& v, ECS::Entity en)
	{
		rjs::Value const& c = v[0]["audioClips"];
		Engine::AudioSource as;
		for (u64 i = 0, asId = 0; i < c.Size(); i += 5)
		{
			// 0 - audio name
			c8 const* name = c[i + 0].GetString();
			Guid id = Engine::AssetManager::Instance()->GetGuid(name);
			as.list[as.id] = Engine::AssetManager::Instance()->GetAudio(id);

			as.list[as.id].m_ID = asId++;

			// 1 - volume
			as.list[as.id].m_Volume = c[i + 1].GetFloat();

			// 2 - loop
			as.list[as.id].m_Loop = c[i + 2].GetBool();

			// 3 - mute
			as.list[as.id].m_Mute = c[i + 3].GetBool();

			// 4 - Channel
			as.list[as.id++].m_Channel = static_cast<Engine::Channel>( c[i + 4].GetUint64() );			
		}
		Coordinator::Instance()->AddComponent(en, as);
	}

#if EDITOR
	void SerializeEntity(ECS::Entity en, s32& serialID, s32 parentID)
	{
		EntityData& ed = Coordinator::Instance()->GetComponent<EntityData>(en);
		ed.id = serialID;
		ed.parentID = parentID;

		ECS::GetSceneGraph().FindImmediateChildren(en);
		std::vector<s32> children = ECS::GetSceneGraph().GetChildren();
		for (s32 child : children)
			SerializeEntity(static_cast<ECS::Entity>(child), ++serialID, ed.id);
	}

	void SaveData(ECS::Entity en, TWriter& writer, u32& id)
	{
		writer.StartObject();
		writer.Key("id");
		writer.Uint(id++);

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
		if (Coordinator::Instance()->HasComponent<Unit>(en))
			WriteUnit(writer, en);
		if (Coordinator::Instance()->HasComponent<ParticleProperties>(en))
			WriteParticleProperty(writer, en);
		if (Coordinator::Instance()->HasComponent<Text>(en))
			WriteTextProperty(writer, en);
		if (Coordinator::Instance()->HasComponent<LogicComponent>(en))
			WriteLogicComponent(writer, en);
		if (Coordinator::Instance()->HasComponent<Engine::AudioSource>(en))
			WriterAudioComponent(writer, en);

		writer.EndObject();
	}

	// To save a prefab
	void SavePrefab(ECS::Entity en)
	{
		rjs::StringBuffer sb{};
		TWriter writer{ sb };

		s32 serialID{};
		SerializeEntity(en, serialID, -1);
		
		ECS::GetSceneGraph().FindChildren(en);
		std::vector<s32> children = ECS::GetSceneGraph().GetChildren();

		writer.StartArray();
		u32 id{ 0 };
		SaveData(en, writer, id);
		std::sort(children.begin(), children.end());
		for (s32 child : children)
			SaveData(child, writer, id);

		writer.EndArray();

		std::string fileName{ "Assets\\Prefab\\" + Coordinator::Instance()->GetComponent<EntityData>(en).tag + ".prefab" };
		std::ofstream ofs{ fileName };
		if (!ofs)
		{
			AL_CORE_WARN("Unable to save into file! {}", fileName);
			return;
		}
		ofs.write(sb.GetString(), sb.GetLength());

		EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(en);
		if (instObjects.count(ed.tag) != 0)
			instObjects[ed.tag] = sb.GetString();
	}
#endif

	void CalculateLocalPosition(ECS::Entity en, s32 parent)
	{
		Transform& trans = Coordinator::Instance()->GetComponent<Transform>(en);
		if (parent != -1)
		{
			Transform const& parentTrans = Coordinator::Instance()->GetComponent<Transform>(parent);
			trans.localPosition = math::mat4::Model({}, { parentTrans.scale.x, parentTrans.scale.y, 1.0f }, trans.rotation).Inverse() * (trans.position - parentTrans.position);
			trans.localRotation = trans.rotation - parentTrans.rotation;
			trans.localScale	= { trans.scale.x / parentTrans.scale.x, trans.scale.y / parentTrans.scale.y };
		}
		else
		{
			trans.localPosition = trans.position;
			trans.localRotation = trans.rotation;
			trans.localScale	= trans.scale;
		}

		ECS::GetSceneGraph().FindImmediateChildren(en);
		std::vector<s32> children{ ECS::GetSceneGraph().GetChildren() };
		for (s32 child : children)
			CalculateLocalPosition(child, en);
	}

	ECS::Entity CreateInstance(std::string const& buffer)
	{
		rjs::Document doc;
		doc.Parse(buffer.c_str());

		using SerialID = s32;

		struct DeserializeID
		{
			ECS::Entity en;
			SerialID parent;
		};

		std::map<SerialID, DeserializeID> entities;

		// Save the prefab data into a seperate entity id starting from (MAX_ENTITIES + 1) - (MAX_ENTITIES + MAX_PREFABS + 1) 
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
			if (v.HasMember("Unit"))
				ReadUnit(v["Unit"], en);
			if (v.HasMember("ParticleProperty"))
				ReadParticleProperty(v["ParticleProperty"], en);
			if (v.HasMember("TextProperty"))
				ReadTextProperty(v["TextProperty"], en);
			if (v.HasMember("LogicComponent"))
				ReadLogicComponent(v["LogicComponent"], en);
			if (v.HasMember("AudioSource"))
				ReadAudioComponent(v["AudioSource"], en);

			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(en);
			entities[ed.id] = { en, ed.parentID };
		}

		for (auto it : entities)
		{
			DeserializeID const& data = it.second;
			s32 const parent = data.parent >= 0 ? static_cast<s32>(entities[data.parent].en) : -1;
			ECS::GetSceneGraph().Push(parent, data.en);
		}

		// calculate the local position of entities
		ECS::Entity en = entities.begin()->second.en;
		CalculateLocalPosition(en, -1);

		return en;
	}

	// Create a clone of a saved prefab
	ECS::Entity Instantiate(std::string const& prefabName)
	{
		// Check to see if an instance of this object is already made
		if (instObjects.find(prefabName) == instObjects.end())
		{	// Making of the prefab and saving it
			std::string fileName{ "Assets\\Prefab\\" + prefabName + ".prefab" };
			c8* buffer = utils::ReadBytes(fileName);

			if (!buffer)
			{
				AL_CORE_CRITICAL("Error: Unable to load prefab: {}", fileName);
				return ECS::MAX_ENTITIES;
			}

			// Save a pointer to where buffer is pointing
			instObjects.insert({ prefabName, buffer });
			Memory::DynamicMemory::Delete(buffer);
		}
		return CreateInstance(instObjects[prefabName]);
	}

	void ClearPrefabCollection(void)
	{
		instObjects.clear();
	}
}