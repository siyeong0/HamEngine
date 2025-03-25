import std.core;
import Common;
import Memory;
import Math;
import ECS;
import SolbitEngine.Renderer;
import SolbitEngine.Resource;
import SolbitEngine.GameObject;
import SolbitEngine.Component;
import SolbitEngine.System.PixelPerfectSpriteRenderSystem;
import SolbitEngine.System.RigidbodyPhysicsSystem;
import SolbitEngine.System.CollisionSystem;

using namespace solbit;

int main(void)
{
	// Initialize ECS
	EntityManager::Initialize();
	ComponentManager::Initialize();

	ComponentManager::Regist<Transform2D>();
	ComponentManager::Regist<RigidBody2D>();
	ComponentManager::Regist<BoxCollider2D>();
	ComponentManager::Regist<PixelPerfectCamera>();
	ComponentManager::Regist<SpriteRenderer>();

	// Initialize Renderer
	Renderer::Initialize();

	// Initialize ResourceManager
	TextureManager::Initialize(Renderer::GetInstance()->GetRenderer());
	if (!TextureManager::GetInstance()->Load(0, "../Resource/Image/Temp/ERROR.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("glorp"), "../Resource/Image/Temp/glorp.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("jonghoon"), "../Resource/Image/Temp/jonghoon.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("stone"), "../Resource/Image/Texture/stone.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}

	PhysicalMaterailManager::Initialize();
	PhysicalMaterailManager::GetInstance()->Add(DEFAULT_ID, PhysicalMaterial());
	PhysicalMaterailManager::GetInstance()->Add(SName("stone"), PhysicalMaterial(2.f, 1.0f, 0.8f, 0.0f));
	PhysicalMaterailManager::GetInstance()->Add(SName("player"), PhysicalMaterial(0.5f, 1.0f, 0.5f, 0.0f));

	// Create Camera
	GameObject mainCamera("MainCamera");
	{
		mainCamera.AddComponent<Transform2D>();
		mainCamera.AddComponent<PixelPerfectCamera>();

		Transform2D& cameraTransform = mainCamera.GetComponent<Transform2D>();
		PixelPerfectCamera& pixelPerfectCamera = mainCamera.GetComponent<PixelPerfectCamera>();
	}
	// Create Floor
	GameObject floor("Floor");
	{
		floor.AddComponent<Transform2D>();
		floor.AddComponent<RigidBody2D>();
		floor.AddComponent<BoxCollider2D>();
		floor.AddComponent<SpriteRenderer>();

		Transform2D& floorTransform = floor.GetComponent<Transform2D>();
		RigidBody2D& floorRigidBody = floor.GetComponent<RigidBody2D>();
		SpriteRenderer& floorSpriteRenderer = floor.GetComponent<SpriteRenderer>();

		floorTransform.Position.Y = -3.f;
		floorTransform.Scale.X = 32.f;
		floorRigidBody.BodyType = EBodyType::Static;
		floorRigidBody.PhysicMaterialId = SName("stone");
		floorSpriteRenderer.SpriteTexId = SName("stone");
	}
	// Create Player
	GameObject player("Player");
	{
		player.AddComponent<Transform2D>();
		player.AddComponent<RigidBody2D>();
		player.AddComponent<BoxCollider2D>();
		player.AddComponent<SpriteRenderer>();

		Transform2D& playerTransform = player.GetComponent<Transform2D>();
		RigidBody2D& playerRigidBody = player.GetComponent<RigidBody2D>();
		SpriteRenderer& playerSpriteRenderer = player.GetComponent<SpriteRenderer>();

		playerTransform.Position.X = 10.f;
		playerRigidBody.Mass = 2.f;
		playerRigidBody.GravityScale = 1.f;
		playerRigidBody.Acceleration += {-500.f, 0.f};
		playerRigidBody.PhysicMaterialId = SName("player");
		playerSpriteRenderer.SpriteTexId = SName("glorp");
	}
	// Create JongHoon
	GameObject jong("JongHoon");
	{
		jong.AddComponent<Transform2D>();
		jong.AddComponent<RigidBody2D>();
		jong.AddComponent<BoxCollider2D>();
		jong.AddComponent<SpriteRenderer>();

		Transform2D& jongTransform = jong.GetComponent<Transform2D>();
		RigidBody2D& jongRigidBody = jong.GetComponent<RigidBody2D>();
		SpriteRenderer& jongSpriteRenderer = jong.GetComponent<SpriteRenderer>();

		jongRigidBody.Acceleration += {+500.f, 0.f};
		jongRigidBody.PhysicMaterialId = SName("player");
		jongSpriteRenderer.SpriteTexId = SName("jonghoon");
	}
	// System set up
	PixelPerfectSpriteRenderSystem spriteRenderSys;
	RigidbodyPhysicsSystem RigidBodyPhysicsSys;
	CollisionSystem CollisionSys;

	Renderer::GetInstance()->AddRTTexture("PixelPerfect", mainCamera.GetComponent<PixelPerfectCamera>().RefResoulution);
	uint64 prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	while (true)
	{
		// Update time
		uint64 currTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		FLOAT dt = (currTime - prevTime) / 1000.f;
		if (dt < RigidbodyPhysicsSystem::FIXED_DELTA_TIME)
		{
			continue;
		}
		static int count = 0;
		std::cout << "f" << count++ << std::endl;
		prevTime = currTime;

		auto& jt = jong.GetComponent<Transform2D>();

		// Collision
		CollisionSys.Execute(floor.GetComponentPack(), jong.GetComponentPack());
		CollisionSys.Execute(player.GetComponentPack(), floor.GetComponentPack());
		CollisionSys.Execute(player.GetComponentPack(), jong.GetComponentPack());

		// Physics
		RigidBodyPhysicsSys.Execute(player.GetComponentPack());
		RigidBodyPhysicsSys.Execute(jong.GetComponentPack());

		// Render
		Renderer::GetInstance()->SetRTTexture("PixelPerfect");
		spriteRenderSys.Execute(player.GetComponentPack(), mainCamera.GetComponentPack());
		spriteRenderSys.Execute(jong.GetComponentPack(), mainCamera.GetComponentPack());
		spriteRenderSys.Execute(floor.GetComponentPack(), mainCamera.GetComponentPack());

		Renderer::GetInstance()->RenderRTTexture("PixelPerfect");
		Renderer::GetInstance()->Render();
	}

	Renderer::Finalize();
	ComponentManager::Finalize();
	EntityManager::Finalize();
	return 0;
}

