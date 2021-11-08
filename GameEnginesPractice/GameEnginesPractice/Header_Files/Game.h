#pragma once

#include "Render/RenderEngine.h"
#include "AdjacentSystems/FileSystem.h"
#include "AdjacentSystems/InputHandler.h"
#include "AdjacentSystems/GameTimer.h"

#include "ResourceManager.h"

#include "CelestialBody.h"
#include "Bullet.h"
#include "Universe.h"

#include <vector>
#include <memory>

#include "ECS/flecs.h"
#include "ECS/GeneralECS.h"
#include "ECS/PhysECS.h"
#include "ECS/MeshECS.h"
#include "ECS/ControlECS.h"


class Game
{
public:
	Game();
	~Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void Run();
	bool Update();

private:
	GameTimer m_Timer;

	std::unique_ptr<flecs::world> m_pECSworld;
	std::unique_ptr<RenderEngine> m_pRenderEngine;
	std::unique_ptr<FileSystem> m_pFileSystem;
	std::unique_ptr<ResourceManager> m_pResourceManager;
	std::unique_ptr<InputHandler> m_pInputHandler;
	//std::unique_ptr<ScriptSystem> m_pScriptSystem;
	//std::unique_ptr<EntityManager> m_pEntityManager;

	void GenerateSolarSystem();
};
