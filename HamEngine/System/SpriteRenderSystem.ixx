module;

import Common;
import ECS.ISystem;
import Renderer;
import Game.GameObject;
import HamEngine.Component;

export module HamEngine.System.SpriteRenderSystem;

export namespace ham
{
	class SpriteRenderSystem final : public ISystem
	{
	public:
		SpriteRenderSystem();
		virtual ~SpriteRenderSystem() = default;

	private:
		
	};
}

namespace ham
{
	SpriteRenderSystem::SpriteRenderSystem()
		: ISystem()
	{

	}

}