module;

#include <SDL2/SDL_mixer.h>

import Common;
import Memory;
import SolbitSTL;
import SolbitEngine.Resource.MusicData;

export module SolbitEngine.Resource.MusicDataManager;

export namespace solbit
{
	class MusicDataManager
	{
	public:
		static MusicDataManager* GetInstance();
		static bool Initialize();
		static void Finalize();

		bool Load(ID id, const std::string& path);
		void Free(ID id);

		inline MusicData& Get(ID id);

	private:
		MusicDataManager() = default;
		~MusicDataManager() = default;
		MusicDataManager(const MusicDataManager&) = delete;
		MusicDataManager(const MusicDataManager&&) = delete;

	private:
		static MusicDataManager* msInstance;

		HashMap<ID, MusicData> mMusicDataMap;
	};
}

namespace solbit
{
	MusicDataManager* MusicDataManager::msInstance = nullptr;

	MusicDataManager* MusicDataManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool MusicDataManager::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new MusicDataManager;

		return msInstance != nullptr;
	}

	void MusicDataManager::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	bool MusicDataManager::Load(ID id, const std::string& path)
	{
		ASSERT(mMusicDataMap.find(id) == mMusicDataMap.end());

		// TODO:
		Mix_Music* music = Mix_LoadMUS(path.c_str());
		MusicData musicData(music);

		mMusicDataMap.insert({ id, musicData });

		return true;
	}

	void MusicDataManager::Free(ID id)
	{
		ASSERT(mMusicDataMap.find(id) != mMusicDataMap.end());

		// TODO:

		mMusicDataMap.erase(id);
	}

	inline MusicData& MusicDataManager::Get(ID id)
	{
		ASSERT(mMusicDataMap.find(id) != mMusicDataMap.end());
		return mMusicDataMap.at(id);
	}
}