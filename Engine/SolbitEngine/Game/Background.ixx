module;

import Common;
import Math;
import SolbitEngine.Component;
import SolbitEngine.Input;
import SolbitEngine.GameObject;
import SolbitEngine.Resource;

export module Game.Background;

export namespace solbit
{
	class Background final : public GameObject
	{
	public:
		Background(ID texId, const FVector2& parallexEffect);

		virtual void Start() override;
		virtual void FixedUpdate() override;
		virtual void Update(FLOAT dt) override;
		virtual void LateUpdate(FLOAT dt) override;
		virtual void OnCollisionEnter(Collision2D& collision) override;
		virtual void OnCollisionExit(Collision2D& collision) override;
		virtual void OnDestroy() override;

	};
}

namespace solbit
{
	Background::Background(ID texId, const FVector2& parallexEffect)
		: GameObject()
	{
		AddComponent<ParallexBackground>();
		ParallexBackground& background = GetComponent<ParallexBackground>();

		background.TexId = texId;
		background.EffectValue = parallexEffect;
	}

	void Background::Start()
	{

	}

	void Background::FixedUpdate()
	{

	}

	void Background::Update(FLOAT dt)
	{

	}

	void Background::LateUpdate(FLOAT dt)
	{

	}

	void Background::OnCollisionEnter(Collision2D& collision)
	{
	}

	void Background::OnCollisionExit(Collision2D& collision)
	{

	}

	void Background::OnDestroy()
	{

	}
}