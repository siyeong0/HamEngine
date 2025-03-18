module;

#include <SDL2\SDL.h>

import std.core;
import Common;

export module Renderer;

export namespace ham
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool Initialize();
		void Finalize();

		void Update(float dt);
		void Render();
	private:
		SDL_Window* mWindow;
		SDL_Renderer* mSDLRenderer;
	};

	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{

	}

	bool Renderer::Initialize()
	{
		// Init SDL
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
			{
				std::cout << "SDL Initialization Fail:\n" << SDL_GetError();
				return false;
			}

			mWindow = SDL_CreateWindow("SDL2 Window",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				300, 300,
				SDL_WINDOW_SHOWN);
			if (!mWindow)
			{
				std::cout << "SDL Initialization Fail:\n" << SDL_GetError();
				SDL_Quit();
				return false;
			}

			mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			if (!mSDLRenderer)
			{
				std::cout << "SDL Renderer Initialization Fail:\n" << SDL_GetError();
				return false;
			}
		}

		return true;
	}

	void Renderer::Finalize()
	{

	}

	void Renderer::Update(float dt)
	{

	}

	void Renderer::Render()
	{
		SDL_RenderPresent(mSDLRenderer);
		SDL_RenderClear(mSDLRenderer);
	}
}