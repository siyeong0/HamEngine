import std.core;
import Common;
import Memory;
import Math;
import ECS;
import SolbitSTL;
import SolbitEngine.Time;
import SolbitEngine.Renderer;
import SolbitEngine.Physics2D;
import SolbitEngine.Audio;
import SolbitEngine.Resource;
import SolbitEngine.Input;
import SolbitEngine.GameObject;
import SolbitEngine.Component;
import SolbitEngine.System.PixelPerfectSpriteRenderSystem;
import SolbitEngine.System.RigidbodyPhysicsSystem;
import SolbitEngine.System.PixelPerfectTilemapRenderSystem;
import SolbitEngine.System.PositionConstraintSystem;
import SolbitEngine.System.ParallexBackgroundSys;
import SolbitEngine.System.AnimationSystem;

import Game.Player;
import Game.Tile;
import Game.Background;

using namespace solbit;

int main(void)
{
	// Initialize ECS
	EntityManager entityManager;
	EntityManager::SetActive(&entityManager);

	ComponentManager::Initialize();
	ComponentManager::Regist<Transform2D>("Transform2D");
	ComponentManager::Regist<RigidBody2D>("RigidBody2D");
	ComponentManager::Regist<BoxCollider2D>("BoxCollider2D");
	ComponentManager::Regist<CapsuleCollider2D>("CapsuleCollider2D");
	ComponentManager::Regist<PixelPerfectCamera>("PixelPerfectCamera");
	ComponentManager::Regist<SpriteRenderer>("SpriteRenderer");
	ComponentManager::Regist<TilemapRenderer>("TilemapRenderer");
	ComponentManager::Regist<PositionConstraint>("PositionConstraint");
	ComponentManager::Regist<ParallexBackground>("ParallexBackground");
	ComponentManager::Regist<Animation>("Animation");
	
	// Initialize Renderer
	Renderer::Initialize();

	// Initialize Audio
	Audio::Initialize();

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
	if (!TextureManager::GetInstance()->Load(SName("dirt"), "../Resource/Image/Tile/dirt.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("tile_map"), "../Resource/Image/Tile/tile_map.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("bgr_natural_0"), "../Resource/Image/Background/Natural/0.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("bgr_natural_1"), "../Resource/Image/Background/Natural/1.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("bgr_natural_2"), "../Resource/Image/Background/Natural/2.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("bgr_natural_3"), "../Resource/Image/Background/Natural/3.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->Load(SName("dancing_rat"), "../Resource/Animation/DancingRat/dancing_rat.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}

	const uint32 PPU = 16;
	SpriteManager::Initialize();
	SpriteManager::GetInstance()->Add(SName("glorp"), Sprite(SName("glorp"), IRectangle(0, 0, 32, 32), PPU));
	SpriteManager::GetInstance()->Add(SName("jonghoon"), Sprite(SName("jonghoon"), IRectangle(0, 0, 128, 128), PPU));
	SpriteManager::GetInstance()->Add(SName("stone"), Sprite(SName("stone"), IRectangle(0, 0, 16, 16), PPU));
	SpriteManager::GetInstance()->Add(SName("dirt"), Sprite(SName("dirt"), IRectangle(0, 0, 16, 16), PPU));
	SpriteManager::GetInstance()->Add(SName("tile_map"), Sprite(SName("tile_map"), IRectangle(16, 16, 16, 16), PPU));

	AnimationDataManager::Initialize();
	AnimationDataManager::GetInstance()->Load(SName("dancing_rat"), "path");

	MusicDataManager::Initialize();
	MusicDataManager::GetInstance()->Load(SName("peaceful"), "../Resource/Music/Peaceful.mp3");

	AudioDataManager::Initialize();
	AudioDataManager::GetInstance()->Load(SName("jump"), "../Resource/Audio/jump.wav");
	AudioDataManager::GetInstance()->Load(SName("rat"), "../Resource/Audio/rat.wav");

	// Initialize physics materials
	PhysicalMaterailManager::Initialize();
	PhysicalMaterailManager::GetInstance()->Add(DEFAULT_ID, PhysicalMaterial());
	PhysicalMaterailManager::GetInstance()->Add(SName("stone"), PhysicalMaterial(2.f, 1.0f, 0.0f));
	PhysicalMaterailManager::GetInstance()->Add(SName("player"), PhysicalMaterial(0.5f, 0.1f, 0.0f));

	// Initialize Input
	Input::Initialize();
	Input::GetInstance()->Regist(SName("MoveLeft"), EInputType::Button);
	Input::GetInstance()->Regist(SName("MoveRight"), EInputType::Button);
	Input::GetInstance()->Regist(SName("Jump"), EInputType::Button);
	Input::GetInstance()->Regist(SName("MouseLeft"), EInputType::Button);
	Input::GetInstance()->Regist(SName("MouseRight"), EInputType::Button);
	Input::GetInstance()->Regist(SName("MouseMove"), EInputType::Vector2);
	Input::GetInstance()->Map(EInputCode::KEY_A, SName("MoveLeft"));
	Input::GetInstance()->Map(EInputCode::KEY_D, SName("MoveRight"));
	Input::GetInstance()->Map(EInputCode::KEY_SPACE, SName("Jump"));
	Input::GetInstance()->Map(EInputCode::MOUSE_LEFTBUTTON, SName("MouseLeft"));
	Input::GetInstance()->Map(EInputCode::MOUSE_RIGHTBUTTON, SName("MouseRight"));
	Input::GetInstance()->Map(EInputCode::MOUSE_MOVE, SName("MouseMove"));

	// Game Objects
	Vector<GameObject*> gameObjects;

	// Create Player
	Player player("Player");
	gameObjects.push_back(&player);

	// Create Camera
	GameObject mainCamera("MainCamera");
	{
		mainCamera.AddComponent<Transform2D>();
		mainCamera.AddComponent<PixelPerfectCamera>();
		mainCamera.AddComponent<PositionConstraint>();

		Transform2D& cameraTransform = mainCamera.GetComponent<Transform2D>();
		PixelPerfectCamera& pixelPerfectCamera = mainCamera.GetComponent<PixelPerfectCamera>();
		PositionConstraint& positionConstraint = mainCamera.GetComponent<PositionConstraint>();

		pixelPerfectCamera.PixelPerUnit = PPU;
		pixelPerfectCamera.RefResoulution = IVector2{ 640, 360 };
		positionConstraint.Source = player.GetEntity();
	}
	gameObjects.push_back(&mainCamera);

	// Create Tilemap
	int XN = 512;
	int YN = 5;
	for (int y = 0; y < YN; ++y)
	{
		for (int x = 0; x < XN; ++x)
		{
			int k = std::rand();
			if (k % 1 == 0)
			{
				Tile* tile = new Tile(IVector2{ x - (XN / 2), y - (YN + 15) });
				gameObjects.push_back(tile);
			}
		}
	}

	// Create parallex background
	Background bg0(SName("bgr_natural_0"), FVector2(0.0f, 0.0f));
	Background bg1(SName("bgr_natural_1"), FVector2(0.1f, 0.0f));
	Background bg2(SName("bgr_natural_2"), FVector2(0.2f, 0.0f));
	Background bg3(SName("bgr_natural_3"), FVector2(0.7f, 0.0f));
	gameObjects.push_back(&bg0);
	gameObjects.push_back(&bg1);
	gameObjects.push_back(&bg2);
	gameObjects.push_back(&bg3);

	// Initialize Phyiscs
	Physics2D::Initialize();
	for (auto obj : gameObjects)
	{
		Physics2D::GetInstance()->AddBody(*obj);
	}

	// Set pixel perfect render target
	Renderer::GetInstance()->AddRTTexture("PixelPerfect", mainCamera.GetComponent<PixelPerfectCamera>().RefResoulution);
	Renderer::GetInstance()->AddRTTexture("ParallexBackground", { 1920, 1080 });

	// Play background music
	Audio::GetInstance()->PlayMusic(MusicDataManager::GetInstance()->Get(SName("peaceful")));

	// System set up
	PixelPerfectSpriteRenderSystem SpriteRenderSys;
	RigidbodyPhysicsSystem RigidBodyPhysicsSys;
	PositionConstraintSystem PosConstraintSys;
	ParallexBackgroundSys BackgroundSys;
	AnimationSystem AnimationSys;

	// Object Start
	for (auto obj : gameObjects)
	{
		obj->Start();
	}

	DOUBLE prevTime = Time::GetCurrentTime();
	FLOAT fixedUpdateTime = 0.0f;
	while (true)
	{
		// Update time
		DOUBLE currTime = Time::GetCurrentTime();
		FLOAT dt = static_cast<FLOAT>(currTime - prevTime);
		prevTime = currTime;

		// Object Fixed Update
		for (auto obj : gameObjects)
		{
			obj->FixedUpdate();
		}

		// Physics
		fixedUpdateTime += dt;
		if (dt >= 1.0f / 60.0f)
		{
			for (auto obj : gameObjects)
			{
				Physics2D::GetInstance()->ApplyToB2Body(*obj);
			}
			Physics2D::GetInstance()->Update(dt);
			for (auto obj : gameObjects)
			{
				Physics2D::GetInstance()->ApplyToSBBody(*obj);
			}
			fixedUpdateTime -= 1.0f / 60.0f;
		}

		// Input
		Input::GetInstance()->Update(dt);

		// Object Update
		for (auto obj : gameObjects)
		{
			obj->Update(dt);
		}

		// Object Late Update
		for (auto obj : gameObjects)
		{
			obj->LateUpdate(dt);
		}

		// Position constraint
		for (auto obj : gameObjects)
		{
			PosConstraintSys.Execute(obj->GetEntity());
		}

		// Animation
		for (auto obj : gameObjects)
		{
			AnimationSys.Execute(obj->GetEntity());
		}

		// Render
		Renderer::GetInstance()->SetRTTexture("ParallexBackground");
		for (auto obj : gameObjects)
		{
			BackgroundSys.Execute(obj->GetEntity(), mainCamera.GetEntity());
		}
		Renderer::GetInstance()->RenderRTTexture("ParallexBackground");

		Renderer::GetInstance()->SetRTTexture("PixelPerfect");
		for (auto obj : gameObjects)
		{
			SpriteRenderSys.Execute(obj->GetEntity(), mainCamera.GetEntity());
		}
		Renderer::GetInstance()->RenderRTTexture("PixelPerfect");

		Renderer::GetInstance()->Render();
	}
	// Object OnDestroy
	for (auto obj : gameObjects)
	{
		obj->OnDestroy();
	}

	Input::Finalize();
	Renderer::Finalize();
	Physics2D::Finalize();
	ComponentManager::Finalize();
	return 0;
}

