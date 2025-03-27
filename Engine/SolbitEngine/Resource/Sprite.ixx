module;

#include <SDL2/SDL.h>

import Common;
import Math;
import SolbitEngine.Resource.Texture;
import SolbitEngine.Resource.TextureManager;

export module SolbitEngine.Resource.Sprite;

export namespace solbit
{
	class Sprite
	{
	public:
		ID TexId;
		uint32 PixelPerUnit;
	public:
		Sprite();
		Sprite(ID texId, uint32 ppu);

		inline Texture& GetTexture();

		inline uint32 GetWidth() const;
		inline uint32 GetHeight() const;
		inline IVector2 GetResoulution() const;
	};
}

namespace solbit
{
	Sprite::Sprite()
		: TexId(0)
		, PixelPerUnit(0)
	{

	}

	Sprite::Sprite(ID texId, uint32 ppu)
		: TexId(texId)
		, PixelPerUnit(ppu)
	{

	}

	inline Texture& Sprite::GetTexture()
	{
		return TextureManager::GetInstance()->Get(TexId);
	}

	inline uint32 Sprite::GetWidth() const
	{
		return TextureManager::GetInstance()->Get(TexId).GetWidth();
	}

	inline uint32 Sprite::GetHeight() const
	{
		return TextureManager::GetInstance()->Get(TexId).GetHeight();
	}

	inline IVector2 Sprite::GetResoulution() const
	{
		return TextureManager::GetInstance()->Get(TexId).GetResoulution();
	}
}