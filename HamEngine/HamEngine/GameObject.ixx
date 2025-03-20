module;

import Common;
import ECS.Entity;

export module HamEngine.GameObject;

export namespace ham
{
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

	private:
		String mName;
		Entity mEntity;
	};
}