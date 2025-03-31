module;

#include <SDL2/SDL.h>
#include <SDL2\SDL_mixer.h>

import Common;
import Math;
import SolbitEngine.Resource;

export module SolbitEngine.Audio;

export namespace solbit
{
	class Audio
	{
	public:
		static Audio* GetInstance();
		static bool Initialize();
		static void Finalize();

		void PlayMusic(MusicData& music);
		void PauseMusic();
		void HaltMusic();

		void Play(ID id);
		void Play(AudioData& audio);
		void PlayChannel(ID id, int32 channel, int loops = 0);
		void PlayChannel(AudioData& audio, int32 channel, int loops = 0);
		void PauseChannel(int32 channel);
		void HaltChannel(int32 channel);

		void SetPosition(int32 channel, FLOAT angle, FLOAT distance);
		
	private:
		Audio() = default;
		~Audio() = default;
		Audio(const Audio&) = delete;
		Audio(const Audio&&) = delete;

	private:
		static Audio* msInstance;
	};
}

namespace solbit
{
	Audio* Audio::msInstance = nullptr;

	Audio* Audio::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool Audio::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new Audio;

		SDL_Init(SDL_INIT_AUDIO);

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
		{
			std::cout << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
			return false;
		}

		Mix_AllocateChannels(32);

		return true;
	}

	void Audio::Finalize()
	{
		Mix_CloseAudio();
		SDL_Quit();

		delete msInstance;
		msInstance = nullptr;
	}

	void Audio::PlayMusic(MusicData& music)
	{
		int r = Mix_PlayMusic(music.Get(), -1);
		//ASSERT(r == 0);
	}

	void Audio::PauseMusic()
	{
		Mix_PauseMusic();
	}

	void Audio::HaltMusic()
	{
		Mix_HaltMusic();
	}

	void Audio::Play(ID id)
	{
		Play(AudioDataManager::GetInstance()->Get(id));
		//ASSERT(r == 0);
	}

	void Audio::Play(AudioData& audio)
	{
		int r = Mix_PlayChannel(-1, audio.Get(), 0);
		//ASSERT(r == 0);
	}

	void Audio::PlayChannel(ID id, int32 channel, int loops)
	{
		PlayChannel(AudioDataManager::GetInstance()->Get(id), channel, loops);
		//ASSERT(r == 0);
	}

	void Audio::PlayChannel(AudioData& audio, int32 channel, int loops)
	{
		int r = Mix_PlayChannel(channel, audio.Get(), loops);
		//ASSERT(r == 0);
	}

	void Audio::PauseChannel(int32 channel)
	{
		Mix_Pause(channel);
	}

	void Audio::HaltChannel(int32 channel)
	{
		Mix_HaltChannel(channel);
	}

	void Audio::SetPosition(int32 channel, FLOAT angle, FLOAT distance)
	{
		ASSERT(distance > 0.0f && distance <= 1.0f);

		int r = Mix_SetPosition(channel, static_cast<int16>(ToDegree(angle)), static_cast<uint8>(distance * 255));
		//ASSERT(r == 0);
	}
}