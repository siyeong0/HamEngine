module;

#include <SDL2/SDL.h>

import Common;
import Memory;
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
		inline Vec2i GetResoulution() const;
		inline uint32 GetPPU() const;
		inline void SetPPU(uint32 v);

	private:
		SDL_Surface* mSurface;
		SDL_Texture* mTexture;

		uint32 mPixelPerUnit;	// Default to texture width;
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
		, mPixelPerUnit(surface->w)
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

	inline Vec2i Texture::GetResoulution() const
	{
		return Vec2i{ mSurface->w, mSurface->h };
	}

	inline uint32 Texture::GetPPU() const
	{
		return mPixelPerUnit;
	}

	inline void Texture::SetPPU(uint32 v)
	{
		mPixelPerUnit = v;
	}
}