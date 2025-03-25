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
		virtual void OnCollision(Collision2D& collision) override;
		virtual void OnDestroy() override;
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
		std::cout << "Player Start" << std::endl;
	}

	void Player::FixedUpdate()
	{
		
	}

	void Player::Update(FLOAT dt)
	{
		Transform2D& transform = GetComponent<Transform2D>();
		RigidBody2D& rigidbody = GetComponent<RigidBody2D>();
		SpriteRenderer& sprite = GetComponent<SpriteRenderer>();
	}

	void Player::LateUpdate(FLOAT dt)
	{
		
	}

	void Player::OnCollision(Collision2D& collision)
	{
		std::cout << "Player On Collision" << std::endl;
	}

	void Player::OnDestroy()
	{
		std::cout << "Player On Destroy" << std::endl;
	}
}