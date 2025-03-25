module;

import Common;
import Memory;
import SolbitSTL;
import SolbitEngine.Resource.PhysicalMaterial;

export module SolbitEngine.Resource.PhysicalMaterailManager;

export namespace solbit
{
	class PhysicalMaterailManager
	{
	public:
		static PhysicalMaterailManager* GetInstance();
		static bool Initialize();
		static void Finalize();

		void Add(ID id, const PhysicalMaterial& physicalMaterial);
		bool Load(ID id, const std::string& path);
		void Free(ID id);

		inline PhysicalMaterial& Get(ID id);

	private:
		PhysicalMaterailManager() = default;
		~PhysicalMaterailManager() = default;
		PhysicalMaterailManager(const PhysicalMaterailManager&) = delete;
		PhysicalMaterailManager(const PhysicalMaterailManager&&) = delete;

	private:
		static PhysicalMaterailManager* msInstance;

		HashMap<ID, PhysicalMaterial, DoNothing> mPhysicalMaterialMap;
	};
}

namespace solbit
{
	PhysicalMaterailManager* PhysicalMaterailManager::msInstance = nullptr;

	PhysicalMaterailManager* PhysicalMaterailManager::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;
	}

	bool PhysicalMaterailManager::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new PhysicalMaterailManager;

		return msInstance != nullptr;
	}

	void PhysicalMaterailManager::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}

	void PhysicalMaterailManager::Add(ID id, const PhysicalMaterial& physicalMaterial)
	{
		ASSERT(mPhysicalMaterialMap.find(id) == mPhysicalMaterialMap.end());

		mPhysicalMaterialMap.insert({ id, physicalMaterial });
	}

	bool PhysicalMaterailManager::Load(ID id, const std::string& path)
	{
		ASSERT(mPhysicalMaterialMap.find(id) == mPhysicalMaterialMap.end());

		// TODO:
		PhysicalMaterial physicalMaterial;

		mPhysicalMaterialMap.insert({ id, physicalMaterial });

		return true;
	}

	void PhysicalMaterailManager::Free(ID id)
	{
		ASSERT(mPhysicalMaterialMap.find(id) != mPhysicalMaterialMap.end());

		// TODO:

		mPhysicalMaterialMap.erase(id);
	}

	inline PhysicalMaterial& PhysicalMaterailManager::Get(ID id)
	{
		ASSERT(mPhysicalMaterialMap.find(id) != mPhysicalMaterialMap.end());
		return mPhysicalMaterialMap[id];
	}
}