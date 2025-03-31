module;

import Common;
import SolbitSTL;
import SolbitEngine.Resource.Tileset;

export module SolbitEngine.Resource.TilesetManager;

export namespace solbit
{
	class TilesetManager
	{
	public:
		static TilesetManager* GetInstance();
		static bool Initialize();
		static void Finalize();

		void Add(ID id, const Tileset& tileset);
		bool Load(ID id, const std::string& path);
		void Free(ID id);

		inline Tileset& Get(ID id);

	private:
		TilesetManager() = default;
		~TilesetManager() = default;
		TilesetManager(const TilesetManager&) = delete;
		TilesetManager(const TilesetManager&&) = delete;

	private:
		static TilesetManager* msInstance;

		HashMap<ID, Tileset> mTilesetMap;
	};
}

namespace solbit
{
	TilesetManager* TilesetManager::msInstance = nullptr;

	TilesetManager* TilesetManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool TilesetManager::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new TilesetManager;

		return msInstance != nullptr;
	}

	void TilesetManager::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	void TilesetManager::Add(ID id, const Tileset& tileset)
	{
		ASSERT(mTilesetMap.find(id) == mTilesetMap.end());

		mTilesetMap.insert({ id, tileset });
	}

	bool TilesetManager::Load(ID id, const std::string& path)
	{
		ASSERT(mTilesetMap.find(id) == mTilesetMap.end());

		// TODO:
		Tileset tileset;

		mTilesetMap.insert({ id, tileset });

		return true;
	}

	void TilesetManager::Free(ID id)
	{
		ASSERT(mTilesetMap.find(id) != mTilesetMap.end());

		// TODO:

		mTilesetMap.erase(id);
	}

	inline Tileset& TilesetManager::Get(ID id)
	{
		ASSERT(mTilesetMap.find(id) != mTilesetMap.end());
		return mTilesetMap.at(id);
	}
}