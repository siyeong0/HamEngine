module;

import Common;
import Memory;
import ECS.ISystem;
import SolbitEngine.Renderer;
import SolbitEngine.GameObject;
import SolbitEngine.Component;

export module SolbitEngine.System.SpriteRenderSystem;

export namespace solbit
{
	class SpriteRenderSystem final : public ISystem
	{
	public:
		SpriteRenderSystem();
		virtual ~SpriteRenderSystem() = default;

	private:
		
	};
}

namespace solbit
{
	SpriteRenderSystem::SpriteRenderSystem()
		: ISystem()
	{

	}

}