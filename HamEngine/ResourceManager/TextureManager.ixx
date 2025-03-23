module;

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"

import Common;
import HamSTL.Utility;
import HamSTL.HashMap;
import ResourceManager.Texture;

export module ResourceManager.TextureManager;

export namespace ham
{
	class TextureManager
	{
	public:
		static TextureManager* GetInstance();
		static bool Initialize(SDL_Renderer* targetRenderer);
		static void Finalize();

		bool LoadTexture(Id id, const std::string& path);
		void FreeTexture(Id id);

		inline Texture& GetTexture(Id id);

	private:
		TextureManager() = default;
		~TextureManager() = default;
		TextureManager(const TextureManager&) = delete;
		TextureManager(const TextureManager&&) = delete;

	private:
		static TextureManager* msInstance;
		
		SDL_Renderer* mTargetRenderer;
		HashMap<Id, Texture, DoNothing> mTextureMap;
	};
}

namespace ham
{
	TextureManager* TextureManager::msInstance = nullptr;

	TextureManager* TextureManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool TextureManager::Initialize(SDL_Renderer* targetRenderer)
	{
		ASSERT(msInstance == nullptr);
		msInstance = new TextureManager;

		ASSERT(targetRenderer != nullptr);
		msInstance->mTargetRenderer = targetRenderer;

		return msInstance != nullptr;
	}

	void TextureManager::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	bool TextureManager::LoadTexture(Id id, const std::string& path)
	{
		ASSERT(mTextureMap.find(id) == mTextureMap.end());

		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface)
			return false;

		SDL_Texture* texture = SDL_CreateTextureFromSurface(mTargetRenderer, surface);
		if (texture == nullptr)
			return false;
		
		mTextureMap.insert({ id, Texture{ surface, texture } });

		return true;
	}

	void TextureManager::FreeTexture(Id id)
	{
		ASSERT(mTextureMap.find(id) != mTextureMap.end());

		SDL_DestroyTexture(mTextureMap[id].Get());
		SDL_FreeSurface(mTextureMap[id].GetSurface());

		mTextureMap.erase(id);
	}

	inline Texture& TextureManager::GetTexture(Id id)
	{
		ASSERT(mTextureMap.find(id) != mTextureMap.end());
		return mTextureMap[id];
	}
}