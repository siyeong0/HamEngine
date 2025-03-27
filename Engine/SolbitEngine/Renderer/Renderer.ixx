module;

#include <SDL2\SDL.h>

import std.core;
import Common;
import Math;
import SolbitSTL;
import SolbitEngine.Resource.Texture;
import SolbitEngine.Resource.TextureManager;

export module SolbitEngine.Renderer;

export namespace solbit
{
	class Renderer
	{
	public:
		static Renderer* GetInstance();
		static bool Initialize();
		static void Finalize();

		void Update(float dt);
		void Render();

		void RenderSprite(Texture& texture, const IRectangle& srcRect, const IRectangle& dstRect, FLOAT angle) const;
		void RenderFixedSprite(Texture& texture, const IRectangle& srcRect, const IRectangle& dstRect) const;
		void DrawPoint(int x, int y);

		inline SDL_Renderer* GetRenderer();

		void AddRTTexture(const String& name, const IVector2& resolution);
		void ClearRTTexture();
		void SetRTTexture(const String& name);
		void RenderRTTexture(const String& name);

		inline IVector2 GetAspectRatio() const;
		inline IVector2 GetRTSize() const;	// RT: Render Target

	private:
		Renderer() = default;
		~Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(const Renderer&&) = delete;

	private:
		static Renderer* msInstance;

		SDL_Window* mWindow;
		SDL_Renderer* mSDLRenderer;
		HashMap<String, Pair<SDL_Texture*, IVector2>> mRTTextures;
		IVector2 mCurrRTResolution;
		IVector2 mResolution;
	};
}

namespace solbit
{
	Renderer* Renderer::msInstance = nullptr;

	Renderer* Renderer::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool Renderer::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new Renderer;

		// TODO: 파일설정에서 로드
		msInstance->mResolution = { 1920, 1080 };

		// Init SDL
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
			{
				std::cout << "SDL Initialization Fail:\n" << SDL_GetError();
				return false;
			}

			msInstance->mWindow = SDL_CreateWindow("SDL2 Window",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				msInstance->mResolution.X, msInstance->mResolution.Y,
				SDL_WINDOW_SHOWN);
			if (!msInstance->mWindow)
			{
				std::cout << "SDL Initialization Fail:\n" << SDL_GetError();
				SDL_Quit();
				return false;
			}

			msInstance->mSDLRenderer = SDL_CreateRenderer(msInstance->mWindow, -1, SDL_RENDERER_ACCELERATED);
			if (!msInstance->mSDLRenderer)
			{
				std::cout << "SDL Renderer Initialization Fail:\n" << SDL_GetError();
				return false;
			}
		}

		return msInstance != nullptr;
	}
	void Renderer::Finalize()
	{
		// Destroy RT Textures
		for (auto& t : msInstance->mRTTextures)
		{
			SDL_DestroyTexture(t.second.first);
		}

		// Destroy SDL
		{
			SDL_DestroyRenderer(msInstance->mSDLRenderer);
			msInstance->mSDLRenderer = nullptr;
			SDL_DestroyWindow(msInstance->mWindow);
			msInstance->mWindow = nullptr;
			SDL_Quit();
		}

		delete msInstance;
		msInstance = nullptr;
	}

	void Renderer::Update(float dt)
	{

	}

	void Renderer::Render()
	{
		SDL_RenderPresent(mSDLRenderer);
		SDL_RenderClear(mSDLRenderer);

		mCurrRTResolution = mResolution;
	}

	void Renderer::RenderSprite(Texture& texture, const IRectangle& srcRect, const IRectangle& dstRect, FLOAT angle) const
	{
		SDL_Rect sdlSrcRect = { srcRect.X, srcRect.Y, srcRect.W, srcRect.H };
		SDL_Rect dstSrcRect = { dstRect.X, dstRect.Y, dstRect.W, dstRect.H };

 		SDL_RenderCopyEx(mSDLRenderer,
			texture.Get(),
			&sdlSrcRect,
			&dstSrcRect,
			ToDegree(angle),
			nullptr,
			SDL_FLIP_NONE);
	}

	void Renderer::RenderFixedSprite(Texture& texture, const IRectangle& srcRect, const IRectangle& dstRect) const
	{
		SDL_Rect sdlSrcRect = { srcRect.X, srcRect.Y, srcRect.W, srcRect.H };
		SDL_Rect dstSrcRect = { dstRect.X, dstRect.Y, dstRect.W, dstRect.H };

		SDL_RenderCopy(mSDLRenderer,
			texture.Get(),
			&sdlSrcRect,
			&dstSrcRect);
	}

	void Renderer::DrawPoint(int x, int y)
	{
		SDL_RenderDrawPoint(mSDLRenderer, x, y);
	}

	inline SDL_Renderer* Renderer::GetRenderer()
	{
		return mSDLRenderer;
	}

	void Renderer::AddRTTexture(const String& name, const IVector2& resolution)
	{
		ASSERT(mRTTextures.find(name) == mRTTextures.end());

		SDL_Texture* renderTarget = SDL_CreateTexture(mSDLRenderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			resolution.X, resolution.Y);
		mRTTextures.insert({ name, {renderTarget, resolution} });
	}

	void Renderer::ClearRTTexture()
	{
		mRTTextures.clear();
	}

	void Renderer::SetRTTexture(const String& name)
	{
		ASSERT(mRTTextures.find(name) != mRTTextures.end());
		SDL_SetRenderTarget(mSDLRenderer, mRTTextures[name].first);
		mCurrRTResolution = mRTTextures[name].second;

		SDL_RenderClear(mSDLRenderer);
	}

	void Renderer::RenderRTTexture(const String& name)
	{
		SDL_SetRenderTarget(mSDLRenderer, nullptr);
		SDL_RenderCopy(mSDLRenderer, mRTTextures[name].first, nullptr, nullptr);
	}

	inline IVector2 Renderer::GetAspectRatio() const
	{
		int gcd = std::gcd(mResolution.X, mResolution.Y);
		ASSERT(gcd > 1);
		return mResolution / gcd;
	}

	inline IVector2 Renderer::GetRTSize() const
	{
		return mCurrRTResolution;
	}
}