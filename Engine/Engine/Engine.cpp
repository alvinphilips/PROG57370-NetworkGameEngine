#include "EngineCore.h"
#include "Engine.h"
#include "EngineTime.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "Entity.h"
#include "InputSystem.h"

#include <thread>

#include "AssetManager.h"
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "Component.h"

#include "NetworkClient.h"
#include "NetworkServer.h"

Engine* Engine::instance = nullptr;

extern void Engine_Register();

void Engine::Initialize()
{
	Engine_Register();

	int serverClientChoice = -1;
	std::cout << "Server [0] or Client [1]: ";
	std::cin >> serverClientChoice;
	(serverClientChoice == 0 ? NetworkServer::Instance().Initialize() : NetworkClient::Instance().Initialize());

	// Load the managers
	AudioSystem::Get().Load("../Assets/AudioSystem.json");
	AssetManager::Get().Load("../Assets/AssetManager.json");

	AudioSystem::Get().Initialize();
	AssetManager::Get().Initialize();

	RenderSystem::Instance().Initialize();
	SceneManager::Get().Load();

	SceneManager::Get().Initialize();
	InputSystem::Instance().registerQuitEventHandler([this] {isRunning = false; });
}

void Engine::Destroy()
{
	Time::Instance().Destroy();
	CollisionSystem::Instance().Destroy();
	SceneManager::Get().Destroy();
	AssetManager::Get().Destroy();
	AudioSystem::Get().Destroy();
	RenderSystem::Instance().Destroy();
	delete instance;
	instance = nullptr;
}

void Engine::GameLoop()
{
	while (isRunning)
	{
		Time::Instance().Update();

		// --------------------- Network Update ---------------------
		NetworkClient::Instance().Update();
		NetworkServer::Instance().Update();

		// --------------------- Pre-update Phase ---------------------
		SceneManager::Get().PreUpdate();

		// --------------------- Update Phase ---------------------
		SceneManager::Get().Update();
		RenderSystem::Instance().Update();
		CollisionSystem::Instance().Update();

		// --------------------- Post-update Phase ---------------------
		SceneManager::Get().PostUpdate();

		// --------------------- Input Phase ---------------------
		InputSystem::Instance().Update();
	}
}
