module;

#include <SDL2/SDL.h>

import Common;
import Math;

export module SolbitEngine.Resource.Texture;

export namespace solbit
{
	class Texture
	{
	public:
		Texture();
		Texture(SDL_Surface* surface, SDL_Texture* texture);
		~Texture();

		inline SDL_Texture* Get();
		inline SDL_Surface* GetSurface();

		inline uint32 GetWidth() const;
		inline uint32 GetHeight() const;
		inline IVector2 GetResoulution() const;

	private:
		SDL_Surface* mSurface;
		SDL_Texture* mTexture;
	};
}

namespace solbit
{
	Texture::Texture()
		: mTexture(nullptr)
	{

	}

	Texture::Texture(SDL_Surface* surface, SDL_Texture* texture)
		: mSurface(surface)
		, mTexture(texture)
	{

	}

	Texture::~Texture()
	{
		mSurface = nullptr;
		mTexture = nullptr;
	}

	inline SDL_Texture* Texture::Get()
	{
		ASSERT(mTexture != nullptr);
		return mTexture;
	}

	inline SDL_Surface* Texture::GetSurface()
	{
		return mSurface;
	}

	inline uint32 Texture::GetWidth() const
	{
		return mSurface->w;
	}

	inline uint32 Texture::GetHeight() const
	{
		return mSurface->h;
	}

	inline IVector2 Texture::GetResoulution() const
	{
		return IVector2{ mSurface->w, mSurface->h };
	}
}