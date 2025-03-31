module;

import Common;
import Math;
import SolbitEngine.Component;
import SolbitEngine.Input;
import SolbitEngine.GameObject;
import SolbitEngine.Resource;

export module Game.Tile;

export namespace solbit
{
	class Tile final : public GameObject
	{
	public:
		Tile(const IVector2& posIdx);

		virtual void Start() override;
		virtual void FixedUpdate() override;
		virtual void Update(FLOAT dt) override;
		virtual void LateUpdate(FLOAT dt) override;
		virtual void OnCollisionEnter(Collision2D& collision) override;
		virtual void OnCollisionExit(GameObject* other) override;
		virtual void OnDestroy() override;

	};
}

namespace solbit
{
	Tile::Tile(const IVector2& posIdx)
		: GameObject()
	{
		AddComponent<Transform2D>();
		AddComponent<RigidBody2D>();
		AddComponent<BoxCollider2D>();
		AddComponent<SpriteRenderer>();

		Transform2D& transform = GetComponent<Transform2D>();
		RigidBody2D& rigidBody = GetComponent<RigidBody2D>();
		BoxCollider2D& boxCollider = GetComponent<BoxCollider2D>();
		SpriteRenderer& spriteRenderer = GetComponent<SpriteRenderer>();

		transform.Position = { static_cast<FLOAT>(posIdx.X), static_cast<FLOAT>(posIdx.Y) };
		transform.Rotation = 0.f;
		transform.Scale = { 1.0f, 1.0f };

		rigidBody.BodyType = EBodyType::Static;
		rigidBody.FreezeRotation = true;
		rigidBody.PhysicMaterialId = SName("stone");

		const Sprite& sprite = SpriteManager::GetInstance()->Get(SName("tile_map"));
		boxCollider.MatchSprite(sprite);

		spriteRenderer.SpriteTexId = SName("tile_map");
	}

	void Tile::Start()
	{
		
	}

	void Tile::FixedUpdate()
	{

	}

	void Tile::Update(FLOAT dt)
	{

	}

	void Tile::LateUpdate(FLOAT dt)
	{

	}

	void Tile::OnCollisionEnter(Collision2D& collision)
	{
	}

	void Tile::OnCollisionExit(GameObject* other)
	{

	}

	void Tile::OnDestroy()
	{

	}
}