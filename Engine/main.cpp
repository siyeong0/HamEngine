import std.core;
import Common;
import Memory;
import Math;
import ECS;
import SolbitSTL;
import SolbitEngine.Renderer;
import SolbitEngine.Physics2D;
import SolbitEngine.Physics2D.CollisionListener2D;
import SolbitEngine.Resource;
import SolbitEngine.Input;
import SolbitEngine.GameObject;
import SolbitEngine.Component;
import SolbitEngine.System.PixelPerfectSpriteRenderSystem;
import SolbitEngine.System.RigidbodyPhysicsSystem;

import SolbitEngine.GameObject.Player;

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
	PhysicalMaterailManager::GetInstance()->Add(SName("stone"), PhysicalMaterial(2.f, 1.0f, 0.0f));
	PhysicalMaterailManager::GetInstance()->Add(SName("player"), PhysicalMaterial(0.5f, 1.0f, 0.0f));

	// Initialize Input
	Input::Initialize();
	Input::GetInstance()->Regist(SName("MoveLeft"), EInputType::Button);
	Input::GetInstance()->Regist(SName("MoveRight"), EInputType::Button);
	Input::GetInstance()->Regist(SName("Jump"), EInputType::Button);
	Input::GetInstance()->Regist(SName("MouseMove"), EInputType::Vector2);
	Input::GetInstance()->Map(EInputCode::KEY_A, SName("MoveLeft"));
	Input::GetInstance()->Map(EInputCode::KEY_D, SName("MoveRight"));
	Input::GetInstance()->Map(EInputCode::KEY_SPACE, SName("Jump"));
	Input::GetInstance()->Map(EInputCode::MOUSE_MOVE, SName("MouseMove"));

	// Game Objects
	Vector<GameObject*> gameObjects;

	// Create Camera
	GameObject mainCamera("MainCamera");
	{
		mainCamera.AddComponent<Transform2D>();
		mainCamera.AddComponent<PixelPerfectCamera>();

		Transform2D& cameraTransform = mainCamera.GetComponent<Transform2D>();
		PixelPerfectCamera& pixelPerfectCamera = mainCamera.GetComponent<PixelPerfectCamera>();
	}
	gameObjects.push_back(&mainCamera);
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

		floorTransform.Position.Y = -8.f;
		floorTransform.Scale.X = 32.f;
		floorRigidBody.BodyType = EBodyType::Static;
		floorRigidBody.PhysicMaterialId = SName("stone");
		floorSpriteRenderer.SpriteTexId = SName("stone");
	}
	gameObjects.push_back(&floor);
	// Create Player
	Player player("Player");
	{
		player.AddComponent<Transform2D>();
		player.AddComponent<RigidBody2D>();
		player.AddComponent<BoxCollider2D>();
		player.AddComponent<SpriteRenderer>();

		Transform2D& playerTransform = player.GetComponent<Transform2D>();
		RigidBody2D& playerRigidBody = player.GetComponent<RigidBody2D>();
		SpriteRenderer& playerSpriteRenderer = player.GetComponent<SpriteRenderer>();

		playerTransform.Position.X = 10.f;
		playerRigidBody.FreezeRotation = true;
		playerRigidBody.Mass = 2.f;
		playerRigidBody.GravityScale = 1.f;
		playerRigidBody.PhysicMaterialId = SName("player");
		playerSpriteRenderer.SpriteTexId = SName("glorp");
	}
	gameObjects.push_back(&player);
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

		jongRigidBody.PhysicMaterialId = SName("player");
		jongSpriteRenderer.SpriteTexId = SName("jonghoon");
	}

	// Initialize Phyiscs
	Physics2D::Initialize();
	CollisionListener2D* collisionListener = new CollisionListener2D;
	Physics2D::GetInstance()->RegistContactListener(collisionListener);
	Physics2D::GetInstance()->AddBody(floor, &floor.GetComponent<Transform2D>(), &floor.GetComponent<RigidBody2D>(), &floor.GetComponent<BoxCollider2D>());
	Physics2D::GetInstance()->AddBody(player, &player.GetComponent<Transform2D>(), &player.GetComponent<RigidBody2D>(), &player.GetComponent<BoxCollider2D>());
	Physics2D::GetInstance()->AddBody(jong, &jong.GetComponent<Transform2D>(), &jong.GetComponent<RigidBody2D>(), &jong.GetComponent<BoxCollider2D>());

	// System set up
	PixelPerfectSpriteRenderSystem spriteRenderSys;
	RigidbodyPhysicsSystem RigidBodyPhysicsSys;

	Renderer::GetInstance()->AddRTTexture("PixelPerfect", mainCamera.GetComponent<PixelPerfectCamera>().RefResoulution);
	uint64 prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	// Object Start
	for (auto& obj : gameObjects)
	{
		obj->Start();
	}
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
		// std::cout << "f" << count++ << std::endl;
		prevTime = currTime;

		auto& jt = jong.GetComponent<Transform2D>();

		// Object Fixed Update
		for (auto& obj : gameObjects)
		{
			obj->FixedUpdate();
		}
		
		// Physics
		Physics2D::GetInstance()->ApplyToB2Body(floor, &floor.GetComponent<Transform2D>(), &floor.GetComponent<RigidBody2D>(), &floor.GetComponent<BoxCollider2D>());
		Physics2D::GetInstance()->ApplyToB2Body(player, &player.GetComponent<Transform2D>(), &player.GetComponent<RigidBody2D>(), &player.GetComponent<BoxCollider2D>());
		Physics2D::GetInstance()->ApplyToB2Body(jong, &jong.GetComponent<Transform2D>(), &jong.GetComponent<RigidBody2D>(), &jong.GetComponent<BoxCollider2D>());
		Physics2D::GetInstance()->Update(1.f / 60.f);
		Physics2D::GetInstance()->ApplyToSBBody(floor, &floor.GetComponent<Transform2D>(), &floor.GetComponent<RigidBody2D>(), &floor.GetComponent<BoxCollider2D>());
		Physics2D::GetInstance()->ApplyToSBBody(player, &player.GetComponent<Transform2D>(), &player.GetComponent<RigidBody2D>(), &player.GetComponent<BoxCollider2D>());
		Physics2D::GetInstance()->ApplyToSBBody(jong, &jong.GetComponent<Transform2D>(), &jong.GetComponent<RigidBody2D>(), &jong.GetComponent<BoxCollider2D>());

		// Input
		Input::GetInstance()->Update(dt);

		// Object Update
		for (auto& obj : gameObjects)
		{
			obj->Update(dt);
		}

		// Object Late Update
		for (auto& obj : gameObjects)
		{
			obj->LateUpdate(dt);
		}

		// Render
		Renderer::GetInstance()->SetRTTexture("PixelPerfect");
		spriteRenderSys.Execute(player.GetComponentPack(), mainCamera.GetComponentPack());
		spriteRenderSys.Execute(jong.GetComponentPack(), mainCamera.GetComponentPack());
		spriteRenderSys.Execute(floor.GetComponentPack(), mainCamera.GetComponentPack());

		Renderer::GetInstance()->RenderRTTexture("PixelPerfect");
		Renderer::GetInstance()->Render();
	}
	// Object OnDestroy
	for (auto& obj : gameObjects)
	{
		obj->OnDestroy();
	}

	Input::Finalize();
	Renderer::Finalize();
	Physics2D::Finalize();
	ComponentManager::Finalize();
	EntityManager::Finalize();
	return 0;
}

