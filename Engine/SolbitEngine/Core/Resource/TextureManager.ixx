module;

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"

import Common;
import Memory;
import SolbitSTL;
import SolbitEngine.Resource.Texture;

export module SolbitEngine.Resource.TextureManager;

export namespace solbit
{
	class TextureManager
	{
	public:
		static TextureManager* GetInstance();
		static bool Initialize(SDL_Renderer* targetRenderer);
		static void Finalize();

		bool Load(ID id, const std::string& path);
		void Free(ID id);

		inline Texture& Get(ID id);

	private:
		TextureManager() = default;
		~TextureManager() = default;
		TextureManager(const TextureManager&) = delete;
		TextureManager(const TextureManager&&) = delete;

	private:
		static TextureManager* msInstance;
		
		SDL_Renderer* mTargetRenderer;
		HashMap<ID, Texture> mTextureMap;
	};
}

namespace solbit
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

	bool TextureManager::Load(ID id, const std::string& path)
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

	void TextureManager::Free(ID id)
	{
		ASSERT(mTextureMap.find(id) != mTextureMap.end());

		SDL_DestroyTexture(mTextureMap[id].Get());
		SDL_FreeSurface(mTextureMap[id].GetSurface());

		mTextureMap.erase(id);
	}

	inline Texture& TextureManager::Get(ID id)
	{
		ASSERT(mTextureMap.find(id) != mTextureMap.end());
		return mTextureMap.at(id);
	}
}