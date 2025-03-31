module;

#include <SDL2/SDL_mixer.h>

import Common;

export module SolbitEngine.Resource.MusicData;

export namespace solbit
{
	class MusicData
	{
	public:
		MusicData();
		MusicData(Mix_Music* sdlMusic);
		~MusicData();

		inline Mix_Music* Get();

	private:
		Mix_Music* mSDLMusic;
	};
}

namespace solbit
{
	MusicData::MusicData()
		: mSDLMusic(nullptr)
	{

	}

	MusicData::MusicData(Mix_Music* sdlMusic)
		: mSDLMusic(sdlMusic)
	{

	}

	MusicData::~MusicData()
	{
		mSDLMusic = nullptr;
	}

	inline Mix_Music* MusicData::Get()
	{
		ASSERT(mSDLMusic != nullptr);
		return mSDLMusic;
	}
}