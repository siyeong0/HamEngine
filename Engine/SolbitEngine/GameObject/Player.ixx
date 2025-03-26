module;

import Common;
import Math;
import SolbitEngine.Component;
import SolbitEngine.Input;
import SolbitEngine.GameObject;

export module SolbitEngine.GameObject.Player;

export namespace solbit
{
	class Player final : public GameObject
	{
	public:
		Player();
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
	Player::Player()
		: GameObject()
	{

	}
	Player::Player(const String& name)
		: GameObject(name)
	{

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