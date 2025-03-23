module;

import Common;
import ECS.Entity;
import ECS.EntityManager;
import ECS.ComponentPack;

export module Game.GameObject;

export namespace ham
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(const String& name);
		~GameObject() = default;

		template <typename T>
		T& AddComponent() const;

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

namespace ham
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
	T& GameObject::AddComponent() const
	{
		EntityManager* entityManager = EntityManager::GetInstance();
		return entityManager->AddComponent<T>(mEntity);
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