module;

import Common;
import SolbitSTL;
import SolbitEngine.Resource.Sprite;

export module SolbitEngine.Resource.SpriteManager;

export namespace solbit
{
	class SpriteManager
	{
	public:
		static SpriteManager* GetInstance();
		static bool Initialize();
		static void Finalize();

		void Add(ID id, const Sprite& sprite);
		bool Load(ID id, const std::string& path);
		void Free(ID id);

		inline Sprite& Get(ID id);

	private:
		SpriteManager() = default;
		~SpriteManager() = default;
		SpriteManager(const SpriteManager&) = delete;
		SpriteManager(const SpriteManager&&) = delete;

	private:
		static SpriteManager* msInstance;

		HashMap<ID, Sprite, DoNothing> mSpriteMap;
	};
}

namespace solbit
{
	SpriteManager* SpriteManager::msInstance = nullptr;

	SpriteManager* SpriteManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool SpriteManager::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new SpriteManager;

		return msInstance != nullptr;
	}

	void SpriteManager::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	void SpriteManager::Add(ID id, const Sprite& sprite)
	{
		ASSERT(mSpriteMap.find(id) == mSpriteMap.end());

		mSpriteMap.insert({ id, sprite });
	}

	bool SpriteManager::Load(ID id, const std::string& path)
	{
		ASSERT(mSpriteMap.find(id) == mSpriteMap.end());

		// TODO:
		Sprite tileset;

		mSpriteMap.insert({ id, tileset });

		return true;
	}

	void SpriteManager::Free(ID id)
	{
		ASSERT(mSpriteMap.find(id) != mSpriteMap.end());

		// TODO:

		mSpriteMap.erase(id);
	}

	inline Sprite& SpriteManager::Get(ID id)
	{
		ASSERT(mSpriteMap.find(id) != mSpriteMap.end());
		return mSpriteMap.at(id);
	}
}