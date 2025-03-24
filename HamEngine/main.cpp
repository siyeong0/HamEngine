import Common;
import std.core;
import Math;
import ECS;
import Renderer;
import ResourceManager.TextureManager;
import Game.GameObject;
import HamEngine.Component.PixelPerfectCamera;
import HamEngine.Component.RigidBody2D;
import HamEngine.Component.SpriteRenderer;
import HamEngine.Component.Transform2D;
import HamEngine.System.PixelPerfectSpriteRenderSystem;

using namespace ham;

int main(void)
{
	// Initialize ECS
	EntityManager::Initialize();
	ComponentManager::Initialize();

	ComponentManager::Regist<Transform2D>();
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
	player.AddComponent<SpriteRenderer>();

	Transform2D& playerTransform = player.GetComponent<Transform2D>();
	SpriteRenderer& playerSpriteRenderer = player.GetComponent<SpriteRenderer>();

	//playerSpriteRenderer.SpriteTexId = HName("glorp");

	GameObject jong("JongHoon");
	jong.AddComponent<Transform2D>();
	jong.AddComponent<SpriteRenderer>();

	Transform2D& jongTransform = jong.GetComponent<Transform2D>();
	SpriteRenderer& jongSpriteRenderer = jong.GetComponent<SpriteRenderer>();

	jongSpriteRenderer.SpriteTexId = HName("jonghoon");

	// System set up
	PixelPerfectSpriteRenderSystem ppsr;
	Renderer::GetInstance()->AddRTTexture("PixelPerfect", pixelPerfectCamera.RefResoulution);
	while (true)
	{
		Renderer::GetInstance()->SetRTTexture("PixelPerfect");
		ppsr.Execute(player.GetComponentPack(), mainCamera.GetComponentPack());
		ppsr.Execute(jong.GetComponentPack(), mainCamera.GetComponentPack());

		Renderer::GetInstance()->RenderRTTexture("PixelPerfect");
		Renderer::GetInstance()->Render();

		playerTransform.Position.X -= 0.0005f;
		playerTransform.Position.Y += 0.0005f;
		playerTransform.Rotation += 0.001f;
		playerTransform.Scale.X += 0.0001f;
		playerTransform.Scale.Y += 0.0001f;

		jongTransform.Position.X += 0.0005f;
		jongTransform.Position.Y -= 0.0005f;
		jongTransform.Rotation -= 0.001f;
		jongTransform.Scale.X -= 0.0001f;
		jongTransform.Scale.Y -= 0.0001f;
	}


	Renderer::Finalize();
	ComponentManager::Finalize();
	EntityManager::Finalize();
	return 0;
}

