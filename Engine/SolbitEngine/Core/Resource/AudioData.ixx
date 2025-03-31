module;

#include <SDL2/SDL_mixer.h>

import Common;

export module SolbitEngine.Resource.AudioData;

export namespace solbit
{
	class AudioData
	{
	public:
		AudioData();
		AudioData(Mix_Chunk* sdlAudio);
		~AudioData();

		inline Mix_Chunk* Get();

	private:
		Mix_Chunk* mSDLAudio;
	};
}

namespace solbit
{
	AudioData::AudioData()
		: mSDLAudio(nullptr)
	{

	}

	AudioData::AudioData(Mix_Chunk* sdlAudio)
		: mSDLAudio(sdlAudio)
	{

	}

	AudioData::~AudioData()
	{
		mSDLAudio = nullptr;
	}

	inline Mix_Chunk* AudioData::Get()
	{
		ASSERT(mSDLAudio != nullptr);
		return mSDLAudio;
	}
}