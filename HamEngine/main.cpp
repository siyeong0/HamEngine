import Common;
import std.core;
import Math;
import ECS;
import Renderer;
import ResourceManager.TextureManager;
import Game.GameObject;
import HamEngine.Component;
import HamEngine.System.PixelPerfectSpriteRenderSystem;
import HamEngine.System.RigidbodyPhysicsSystem;

using namespace ham;

int main(void)
{
	// Initialize ECS
	EntityManager::Initialize();
	ComponentManager::Initialize();

	ComponentManager::Regist<Transform2D>();
	ComponentManager::Regist<RigidBody2D>();
	ComponentManager::Regist<PixelPerfectCamera>();
	ComponentManager::Regist<SpriteRenderer>();

	// Initialize Renderer
	Renderer::Initialize();

	// Initialize ResourceManager
	TextureManager::Initialize(Renderer::GetInstance()->GetRenderer());
	if (!TextureManager::GetInstance()->LoadTexture(0, "../Resource/Image/Temp/ERROR.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->LoadTexture(HName("glorp"), "../Resource/Image/Temp/glorp.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}
	if (!TextureManager::GetInstance()->LoadTexture(HName("jonghoon"), "../Resource/Image/Temp/jonghoon.png"))
	{
		std::cout << "Image Load Failed." << std::endl;
		ASSERT(false);
		return 1;
	}

	// Create Camera
	GameObject mainCamera("MainCamera");
	mainCamera.AddComponent<Transform2D>();
	mainCamera.AddComponent<PixelPerfectCamera>();

	Transform2D& cameraTransform = mainCamera.GetComponent<Transform2D>();
	PixelPerfectCamera& pixelPerfectCamera = mainCamera.GetComponent<PixelPerfectCamera>();

	// Create Player
	GameObject player("Player");
	player.AddComponent<Transform2D>();
	player.AddComponent<RigidBody2D>();
	player.AddComponent<SpriteRenderer>();

	Transform2D& playerTransform = player.GetComponent<Transform2D>();
	RigidBody2D& playerRigidBody = player.GetComponent<RigidBody2D>();
	SpriteRenderer& playerSpriteRenderer = player.GetComponent<SpriteRenderer>();

	playerTransform.Position.X = 2.0f;
	playerRigidBody.GravityScale = 0.2f;
	playerSpriteRenderer.SpriteTexId = HName("glorp");

	GameObject jong("JongHoon");
	jong.AddComponent<Transform2D>();
	jong.AddComponent<RigidBody2D>();
	jong.AddComponent<SpriteRenderer>();

	Transform2D& jongTransform = jong.GetComponent<Transform2D>();
	RigidBody2D& jongRigidBody = jong.GetComponent<RigidBody2D>();
	SpriteRenderer& jongSpriteRenderer = jong.GetComponent<SpriteRenderer>();

	jongSpriteRenderer.SpriteTexId = HName("jonghoon");

	// System set up
	PixelPerfectSpriteRenderSystem ppsr;
	RigidbodyPhysicsSystem rbp;
	Renderer::GetInstance()->AddRTTexture("PixelPerfect", pixelPerfectCamera.RefResoulution);

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
		prevTime = currTime;

		// Physics
		rbp.Execute(player.GetComponentPack());
		rbp.Execute(jong.GetComponentPack());

		// Render
		Renderer::GetInstance()->SetRTTexture("PixelPerfect");
		ppsr.Execute(player.GetComponentPack(), mainCamera.GetComponentPack());
		ppsr.Execute(jong.GetComponentPack(), mainCamera.GetComponentPack());

		Renderer::GetInstance()->RenderRTTexture("PixelPerfect");
		Renderer::GetInstance()->Render();

		// Event
		//playerTransform.Position.X -= 0.0005f;
		//playerTransform.Position.Y += 0.0005f;
		//playerTransform.Rotation += 0.001f;
		//playerTransform.Scale.X += 0.0001f;
		//playerTransform.Scale.Y += 0.0001f;

		//jongTransform.Position.X += 0.0005f;
		//jongTransform.Position.Y -= 0.0005f;
		//jongTransform.Rotation -= 0.001f;
		//jongTransform.Scale.X -= 0.0001f;
		//jongTransform.Scale.Y -= 0.0001f;
	}


	Renderer::Finalize();
	ComponentManager::Finalize();
	EntityManager::Finalize();
	return 0;
}

