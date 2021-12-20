#pragma once

#include "Render/RenderEngine.h"
#include "AdjacentSystems/FileSystem.h"
#include "AdjacentSystems/InputHandler.h"
#include "AdjacentSystems/GameTimer.h"
#include "AdjacentSystems/ProjectLoader.h"
#include "WindowManagement/EditorWindow.h"

#include "ScriptSystem/EntityManager.h"
#include "ScriptSystem/ScriptManager.h"
#include "ResourceManager.h"

#include <vector>
#include <memory>

#include "flecs.h"
#include "ECS/GeneralECS.h"
#include "ECS/PhysECS.h"
#include "ECS/MeshECS.h"
#include "ECS/ControlECS.h"
#include "ECS/KinematicsECS.h"
#include "ECS/ScriptECS.h"


class Game
{
public:
	Game();
	~Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void Run();
	bool Update();
	bool UpdateAllSystems();
	bool UpdateECSSystems();

private:

	bool LoadGameWorld();
	GameTimer m_Timer;

	std::unique_ptr<flecs::world> m_pECSworld;
	std::unique_ptr<RenderEngine> m_pRenderEngine;
	std::unique_ptr<FileSystem> m_pFileSystem;
	std::unique_ptr<ResourceManager> m_pResourceManager;
	std::unique_ptr<InputHandler> m_pInputHandler;
	std::unique_ptr<ProjectLoader> m_pProjectLoader;
	std::unique_ptr<EditorWindow> m_pEditorWindow;
	std::unique_ptr<ScriptManager> m_pScriptSystem;
	std::unique_ptr<EntityManager> m_pEntityManager;
	//std::unique_ptr<ScriptSystem> m_pScriptSystem;
	//std::unique_ptr<EntityManager> m_pEntityManager;

	void GenerateSolarSystem();
};
