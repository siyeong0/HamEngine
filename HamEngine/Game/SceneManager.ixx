module;

import Common;
import Math;
import Game.Scene;
import Game.GameObject;

export module Game.SceneManager;

export namespace ham
{
	// TODO: 
	class SceneManager
	{
	public:
		static SceneManager* GetInstance();
		static bool Initialize();
		static void Finalize();

		const Scene* GetActiveScene() const;

	private:
		SceneManager() = default;
		~SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager(const SceneManager&&) = delete;

	private:
		static SceneManager* msInstance;

		Scene* mCurrScene;
	};

	const GameObject* GetMainCamera()
	{
		return SceneManager::GetInstance()->GetActiveScene()->GetMainCamera();
	}
}

namespace ham
{
	SceneManager* SceneManager::msInstance = nullptr;

	SceneManager* SceneManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool SceneManager::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new SceneManager;

		return msInstance != nullptr;
	}
	void SceneManager::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	const Scene* SceneManager::GetActiveScene() const
	{
		ASSERT(mCurrScene != nullptr);
		return mCurrScene;
	}
}