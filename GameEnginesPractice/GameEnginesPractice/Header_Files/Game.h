#pragma once

#include "RenderEngine.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "InputHandler.h"
#include "GameTimer.h"
#include "CelestialBody.h"
#include "Bullet.h"
#include "Universe.h"
#include <vector>


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
	RenderEngine* m_pRenderEngine;
	FileSystem* m_pFileSystem;
	ResourceManager* m_pResourceManager;
	InputHandler* m_pInputHandler;
	
	std::vector<std::unique_ptr<CelestialBody>> solarSystem;
	std::vector<Bullet*> bullets;
	int t = 0;
	int cd = 100;
	void GenerateSolarSystem();
};
