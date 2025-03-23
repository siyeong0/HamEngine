module;

import Common;
import ECS.ISystem;
import Renderer;
import Game.GameObject;
import Game.SceneManager;
import HamEngine.Component.Transform2D;
import HamEngine.Component.SpriteRenderer;

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