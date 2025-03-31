module;

import Common;
import Math;
import SolbitEngine.Component;
import SolbitEngine.Input;
import SolbitEngine.GameObject;
import SolbitEngine.Resource;

export module Game.Player;

export namespace solbit
{
	class Player final : public GameObject
	{
	public:
		Player(const String& name);

		virtual void Start() override;
		virtual void FixedUpdate() override;
		virtual void Update(FLOAT dt) override;
		virtual void LateUpdate(FLOAT dt) override;
		virtual void OnCollisionEnter(Collision2D& collision) override;
		virtual void OnCollisionExit(Collision2D& collision) override;
		virtual void OnDestroy() override;
	private:
		bool mbOnGround;
	};
}

namespace solbit
{
	Player::Player(const String& name)
		: GameObject(name)
	{
		AddComponent<Transform2D>();
		AddComponent<RigidBody2D>();
		AddComponent<BoxCollider2D>();
		AddComponent<SpriteRenderer>();

		Transform2D& transform = GetComponent<Transform2D>();
		RigidBody2D& rigidBody = GetComponent<RigidBody2D>();
		BoxCollider2D& boxCollider = GetComponent<BoxCollider2D>();
		SpriteRenderer& spriteRenderer = GetComponent<SpriteRenderer>();

		transform.Position = { 0.0f, 0.0f };
		transform.Rotation = 0.f;
		transform.Scale = { 1.0f, 1.0f };

		rigidBody.FreezeRotation = true;
		rigidBody.Mass = 2.f;
		rigidBody.GravityScale = 1.f;
		rigidBody.PhysicMaterialId = SName("player");

		const Sprite& sprite = SpriteManager::GetInstance()->Get(SName("glorp"));
		boxCollider.MatchSprite(sprite);

		spriteRenderer.SpriteTexId = SName("glorp");
	}

	void Player::Start()
	{
		mbOnGround = false;
	}

	void Player::FixedUpdate()
	{
		
	}

	void Player::Update(FLOAT dt)
	{
		Transform2D& transform = GetComponent<Transform2D>();
		RigidBody2D& rigidbody = GetComponent<RigidBody2D>();
		SpriteRenderer& sprite = GetComponent<SpriteRenderer>();

		if (Input::GetInstance()->GetButtonState(SName("MoveLeft")))
		{
			rigidbody.Velocity.X = -5.0f;
		}
		if (Input::GetInstance()->GetButtonState(SName("MoveRight")))
		{
			rigidbody.Velocity.X = 5.0f;
		}
		if (mbOnGround && Input::GetInstance()->GetButtonPressed(SName("Jump")))
		{
			rigidbody.Velocity.Y += 10.0f;
		}
	}

	void Player::LateUpdate(FLOAT dt)
	{
		
	}

	void Player::OnCollisionEnter(Collision2D& collision)
	{
		if (std::fabs(collision.ContactNormal.Dot(FVector2{ 0.0f, 1.0f })) > std::cosf(PI / 4.f))
		{
			mbOnGround = true;
		}
	}

	void Player::OnCollisionExit(Collision2D& collision)
	{
		mbOnGround = false;
	}

	void Player::OnDestroy()
	{
		std::cout << "Player On Destroy" << std::endl;
	}
}