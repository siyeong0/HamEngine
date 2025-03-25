module;

import Common;
import Memory;
import SolbitEngine.GameObject;

export module SolbitEngine.Scene;

export namespace solbit
{
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;
		Scene(const Scene&) = delete;
		Scene(const Scene&&) = delete;

		const GameObject* GetMainCamera() const;

	private:
		GameObject* mMainCamera;
	};
}

namespace solbit
{
	const GameObject* Scene::GetMainCamera() const
	{
		ASSERT(mMainCamera != nullptr);
		return mMainCamera;
	}
}