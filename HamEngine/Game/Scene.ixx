module;

import Common;
import Game.GameObject;

export module Game.Scene;

export namespace ham
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

namespace ham
{
	const GameObject* Scene::GetMainCamera() const
	{
		ASSERT(mMainCamera != nullptr);
		return mMainCamera;
	}
}