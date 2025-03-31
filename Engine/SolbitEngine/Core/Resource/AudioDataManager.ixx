module;

#include <SDL2/SDL_mixer.h>

import Common;
import Memory;
import SolbitSTL;
import SolbitEngine.Resource.AudioData;

export module SolbitEngine.Resource.AudioDataManager;

export namespace solbit
{
	class AudioDataManager
	{
	public:
		static AudioDataManager* GetInstance();
		static bool Initialize();
		static void Finalize();

		bool Load(ID id, const std::string& path);
		void Free(ID id);

		inline AudioData& Get(ID id);

	private:
		AudioDataManager() = default;
		~AudioDataManager() = default;
		AudioDataManager(const AudioDataManager&) = delete;
		AudioDataManager(const AudioDataManager&&) = delete;

	private:
		static AudioDataManager* msInstance;

		HashMap<ID, AudioData> mAudioDataMap;
	};
}

namespace solbit
{
	AudioDataManager* AudioDataManager::msInstance = nullptr;

	AudioDataManager* AudioDataManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool AudioDataManager::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new AudioDataManager;

		return msInstance != nullptr;
	}

	void AudioDataManager::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	bool AudioDataManager::Load(ID id, const std::string& path)
	{
		ASSERT(mAudioDataMap.find(id) == mAudioDataMap.end());

		// TODO:
		Mix_Chunk* audio = Mix_LoadWAV(path.c_str());
		AudioData audioData(audio);

		mAudioDataMap.insert({ id, audioData });

		return true;
	}

	void AudioDataManager::Free(ID id)
	{
		ASSERT(mAudioDataMap.find(id) != mAudioDataMap.end());

		// TODO:

		mAudioDataMap.erase(id);
	}

	inline AudioData& AudioDataManager::Get(ID id)
	{
		ASSERT(mAudioDataMap.find(id) != mAudioDataMap.end());
		return mAudioDataMap.at(id);
	}
}