module;

import Common;
import Memory;
import ECS;

export module SolbitEngine.GameObject;

export namespace solbit
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(const String& name);
		~GameObject() = default;

		template <typename T>
		void AddComponent() const;

		template <typename T>
		T& GetComponent() const;

		ComponentPack GetComponentPack() const;

		const String& GetName() const;
		void SetName(const String& name);

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
	{

	}

	GameObject::GameObject(const String& name)
		: mName(name)
		, mEntity(EntityManager::GetInstance()->CreateEntity())
	{

	}

	template <typename T>
	void GameObject::AddComponent() const
	{
		EntityManager* entityManager = EntityManager::GetInstance();
		T& component = entityManager->AddComponent<T>(mEntity);
		new(&component) (T);
	}

	template <typename T>
	T& GameObject::GetComponent() const
	{
		EntityManager* entityManager = EntityManager::GetInstance();
		ComponentPack componentPack = entityManager->GetComponentPack(mEntity);

		return componentPack.GetComponent<T>();
	}

	ComponentPack GameObject::GetComponentPack() const
	{
		EntityManager* entityManager = EntityManager::GetInstance();
		ComponentPack componentPack = entityManager->GetComponentPack(mEntity);

		return componentPack;
	}

	const String& GameObject::GetName() const
	{
		return mName;
	}

	void GameObject::SetName(const String& name)
	{
		mName = name;
	}
}