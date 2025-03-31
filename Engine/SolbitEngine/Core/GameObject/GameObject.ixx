module;

import Common;
import Memory;
import ECS;
import SolbitEngine.Physics2D.Collision2D;

export module SolbitEngine.GameObject;

export namespace solbit
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(const String& name);
		virtual ~GameObject() = default;

		virtual void Start();
		virtual void FixedUpdate();
		virtual void Update(FLOAT dt);
		virtual void LateUpdate(FLOAT dt);
		virtual void OnCollisionEnter(Collision2D& collision);
		virtual void OnCollisionExit(Collision2D& collision);
		virtual void OnDestroy();

		template <typename T>
		void AddComponent() const;

		template <typename T>
		T& GetComponent() const;

		template <typename T>
		bool HasComponent() const;

		inline const String& GetName() const;
		inline void SetName(const String& name);
		inline const Entity GetEntity() const;

	private:
		String mName;
		Entity mEntity;
	};
}

namespace solbit
{
	static uint32 sDefaultNameCount = 0;

	GameObject::GameObject()
		: mName("GameObject_" + std::to_string(sDefaultNameCount++))
		, mEntity(EntityManager::GetActive()->CreateEntity())
	{

	}

	GameObject::GameObject(const String& name)
		: mName(name)
		, mEntity(EntityManager::GetActive()->CreateEntity())
	{

	}

	void GameObject::Start()
	{

	}

	void GameObject::FixedUpdate()
	{

	}

	void GameObject::Update(FLOAT dt)
	{

	}

	void GameObject::LateUpdate(FLOAT dt)
	{

	}

	void GameObject::OnCollisionEnter(Collision2D& collision)
	{

	}

	void GameObject::OnCollisionExit(Collision2D& collision)
	{

	}

	void GameObject::OnDestroy()
	{

	}

	template <typename T>
	void GameObject::AddComponent() const
	{
		EntityManager::GetActive()->AddComponent<T>(mEntity);
	}

	template <typename T>
	T& GameObject::GetComponent() const
	{
		return EntityManager::GetActive()->GetComponent<T>(mEntity);
	}

	template <typename T>
	bool GameObject::HasComponent() const
	{
		return EntityManager::GetActive()->HasComponent<T>(mEntity);
	}

	inline const String& GameObject::GetName() const
	{
		return mName;
	}

	inline void GameObject::SetName(const String& name)
	{
		mName = name;
	}

	inline const Entity GameObject::GetEntity() const
	{
		return mEntity;
	}
}