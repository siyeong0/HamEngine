module;

import Common;
import Memory;
import ECS;
import SolbitEngine.Component;
import SolbitEngine.Resource;

export module SolbitEngine.System.AnimationSystem;

export namespace solbit
{
	class AnimationSystem : public ISystem
	{
	public:
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;

		void Execute(const Entity entity);
	};
}

namespace solbit
{
	void AnimationSystem::OnCreate()
	{

	}

	void AnimationSystem::OnUpdate()
	{

	}

	void AnimationSystem::OnDestroy()
	{

	}

	void AnimationSystem::Execute(const Entity entity)
	{
		// TODO: 임시 코드
		if (!EntityManager::GetActive()->HasComponent<Animation>(entity) || !EntityManager::GetActive()->HasComponent<SpriteRenderer>(entity))
		{
			return;
		}
		Animation& animation = EntityManager::GetActive()->GetComponent<Animation>(entity);
		const AnimationData& animData = AnimationDataManager::GetInstance()->Get(animation.AnimId);
		const AnimFrameData& frameData = animData.SpriteVec[animation.AnimIdx];
		animation.Time += 1.0f / 60.0f;

		if (frameData.TimeOffset <= animation.Time)
		{
			++animation.AnimIdx;
			if (animation.AnimIdx == animData.SpriteVec.size())
			{
				animation.AnimIdx = 0;
				animation.Time -= animData.TimeLength;
			}

			SpriteRenderer& sprite = EntityManager::GetActive()->GetComponent<SpriteRenderer>(entity);
			sprite.SpriteTexId = animData.SpriteVec[animation.AnimIdx].SpriteId;
		}
	}
}