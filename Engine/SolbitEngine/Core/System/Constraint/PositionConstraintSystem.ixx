module;

import Common;
import Memory;
import ECS;
import SolbitEngine.Component;

export module SolbitEngine.System.PositionConstraintSystem;

export namespace solbit
{ 
	class PositionConstraintSystem : public ISystem 
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
	void PositionConstraintSystem::OnCreate()
	{

	}

	void PositionConstraintSystem::OnUpdate()
	{

	}

	void PositionConstraintSystem::OnDestroy()
	{

	}

	void PositionConstraintSystem::Execute(const Entity entity)
	{
		// TODO: 임시 코드
		if (!EntityManager::GetActive()->HasComponent<Transform2D>(entity) || !EntityManager::GetActive()->HasComponent<PositionConstraint>(entity))
		{
			return;
		}
		Transform2D& transform = EntityManager::GetActive()->GetComponent<Transform2D>(entity);
		const PositionConstraint& posConstraint = EntityManager::GetActive()->GetComponent<PositionConstraint>(entity);
		const Transform2D& sourceTransform = EntityManager::GetActive()->GetComponent<Transform2D>(posConstraint.Source);

		transform.Position = sourceTransform.Position;
		transform.Position.Y += 3.0f;
	}
}