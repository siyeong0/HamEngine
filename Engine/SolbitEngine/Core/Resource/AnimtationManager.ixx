module;

import Common;
import Memory;
import SolbitSTL;
import SolbitEngine.Resource.AnimationData;
import SolbitEngine.Resource.Sprite;
import SolbitEngine.Resource.SpriteManager;

export module SolbitEngine.Resource.AnimationDataManager;

export namespace solbit
{
	class AnimationDataManager
	{
	public:
		static AnimationDataManager* GetInstance();
		static bool Initialize();
		static void Finalize();

		bool Load(ID id, const std::string& path);
		void Free(ID id);

		inline const AnimationData& Get(ID id) const;

	private:
		AnimationDataManager() = default;
		~AnimationDataManager() = default;
		AnimationDataManager(const AnimationDataManager&) = delete;
		AnimationDataManager(const AnimationDataManager&&) = delete;

	private:
		static AnimationDataManager* msInstance;

		HashMap<ID, AnimationData> mAnimationDataMap;
	};
}

namespace solbit
{
	AnimationDataManager* AnimationDataManager::msInstance = nullptr;

	AnimationDataManager* AnimationDataManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool AnimationDataManager::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new AnimationDataManager;

		return msInstance != nullptr;
	}

	void AnimationDataManager::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	bool AnimationDataManager::Load(ID id, const std::string& path)
	{
		ASSERT(mAnimationDataMap.find(id) == mAnimationDataMap.end());

		// TODO:
		AnimationData animationData;
		for (int y = 0; y < 18; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				if (y == 17 && x == 6)
				{
					break;
				}
				Sprite sprite;
				sprite.TexId = SName("dancing_rat");
				sprite.Rectangle = {128 * x, 128 * y, 128, 128};
				sprite.PixelPerUnit = 16;
				SpriteManager::GetInstance()->Add(SName("dancing_rat_"+ std::to_string(y * 8 + x)), sprite);

				AnimFrameData frame;
				frame.SpriteId = SName("dancing_rat_" + std::to_string(y * 8 + x));
				frame.TimeOffset = (1.0f / 30.0f) * (y * 8 + x + 1);
				animationData.SpriteVec.push_back(frame);
			}
		}
		animationData.TimeLength = animationData.SpriteVec.back().TimeOffset;
		mAnimationDataMap.insert({ id, std::move(animationData) });

		return true;
	}

	void AnimationDataManager::Free(ID id)
	{
		ASSERT(mAnimationDataMap.find(id) != mAnimationDataMap.end());

		// TODO:

		mAnimationDataMap.erase(id);
	}

	inline const AnimationData& AnimationDataManager::Get(ID id) const
	{
		ASSERT(mAnimationDataMap.find(id) != mAnimationDataMap.end());
		return mAnimationDataMap.at(id);
	}
}