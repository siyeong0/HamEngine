module;

import Common;
import Math;
import SolbitEngine.Audio;
import SolbitEngine.Component;
import SolbitEngine.Input;
import SolbitEngine.GameObject;
import SolbitEngine.Resource;

export module Game.Player;

export namespace solbit
{
	class Player : public GameObject
	{
	public:
		Player(const String& name);
		virtual ~Player() = default;

		virtual void Start() override;
		virtual void FixedUpdate() override;
		virtual void Update(FLOAT dt) override;
		virtual void LateUpdate(FLOAT dt) override;
		virtual void OnCollisionEnter(Collision2D& collision) override;
		virtual void OnCollisionExit(GameObject* other) override;
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
		AddComponent<Animation>();

		Transform2D& transform = GetComponent<Transform2D>();
		RigidBody2D& rigidBody = GetComponent<RigidBody2D>();
		BoxCollider2D& boxCollider = GetComponent<BoxCollider2D>();
		SpriteRenderer& spriteRenderer = GetComponent<SpriteRenderer>();
		Animation& animation = GetComponent<Animation>();

		transform.Position = { 0.0f, 0.0f };
		transform.Rotation = 0.f;
		transform.Scale = { 1.0f, 1.0f };

		rigidBody.FreezeRotation = true;
		rigidBody.Mass = 2.f;
		rigidBody.GravityScale = 1.f;
		rigidBody.PhysicMaterialId = SName("player");

		const Sprite& sprite = SpriteManager::GetInstance()->Get(SName("dancing_rat_0"));
		boxCollider.MatchSprite(sprite);

		spriteRenderer.SpriteTexId = SName("dancing_rat_0");
		animation.AnimId = SName("dancing_rat");
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

		if (Input::GetInstance()->GetButtonPressed(SName("MouseLeft")))
		{
			Audio::GetInstance()->PlayChannel(SName("rat"), 8);
		}
		if (Input::GetInstance()->GetButtonReleased(SName("MouseLeft")))
		{
			// Audio::GetInstance()->PauseChannel(8);
		}
		if (Input::GetInstance()->GetButtonPressed(SName("MouseRight")))
		{

		}

		if (Input::GetInstance()->GetButtonState(SName("MoveLeft")))
		{
			rigidbody.Velocity.X = -5.0f;
		}
		if (Input::GetInstance()->GetButtonReleased(SName("MoveLeft")))
		{
			rigidbody.Velocity.X = 0.0f;
		}
		if (Input::GetInstance()->GetButtonState(SName("MoveRight")))
		{
			rigidbody.Velocity.X = 5.0f;
		}
		if (Input::GetInstance()->GetButtonReleased(SName("MoveRight")))
		{
			rigidbody.Velocity.X = 0.0f;
		}
		if (mbOnGround && Input::GetInstance()->GetButtonPressed(SName("Jump")))
		{
			rigidbody.Velocity.Y += 10.0f;
			Audio::GetInstance()->Play(SName("jump"));
		}
		std::cout << mbOnGround << std::endl;
	}

	void Player::LateUpdate(FLOAT dt)
	{

	}

	void Player::OnCollisionEnter(Collision2D& collision)
	{
		if (collision.ContactPoint.Y < GetComponent<Transform2D>().Position.Y)
		{
			mbOnGround = true;
		}
	}

	void Player::OnCollisionExit(GameObject* other)
	{
		mbOnGround = false;
	}

	void Player::OnDestroy()
	{
		std::cout << "Player On Destroy" << std::endl;
	}
}