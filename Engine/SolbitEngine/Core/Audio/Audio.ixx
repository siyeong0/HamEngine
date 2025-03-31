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

		void PlayChannel(AudioData& audio, uint32 channel, int loops);
		void PauseChannel(uint32 channel);
		void HaltChannel(uint32 channel);

		void SetPosition(uint32 channel, FLOAT angle, FLOAT distance);
		
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
			fprintf(stderr, "Failed to initialize SDL_mixer: %s\n", Mix_GetError());
			return false;
		}

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
		ASSERT(r == 0);
	}

	void Audio::PauseMusic()
	{
		Mix_PauseMusic();
	}

	void Audio::HaltMusic()
	{
		Mix_HaltMusic();
	}

	void Audio::PlayChannel(AudioData& audio, uint32 channel, int loops)
	{
		int r = Mix_PlayChannel(channel, audio.Get(), loops);
		ASSERT(r == 0);
	}

	void Audio::PauseChannel(uint32 channel)
	{
		Mix_PauseAudio(channel);
	}

	void Audio::HaltChannel(uint32 channel)
	{
		Mix_HaltChannel(channel);
	}

	void Audio::SetPosition(uint32 channel, FLOAT angle, FLOAT distance)
	{
		ASSERT(distance > 0.0f && distance <= 1.0f);

		int r = Mix_SetPosition(channel, static_cast<int16>(ToDegree(angle)), static_cast<uint8>(distance * 255));
		ASSERT(r == 0);
	}
}