#include "Game.h"

Game::Game() :
	m_pRenderEngine(nullptr)
{
	m_pRenderEngine = new RenderEngine();

	GenerateSolarSystem();

	m_Timer.Start();
}

Game::~Game()
{
}

void Game::Run()
{
	m_Timer.Reset();

	while (true)
	{
		m_pRenderEngine->GetRT()->RC_BeginFrame();

		m_Timer.Tick();
		if (!Update())
			break;

		m_pRenderEngine->GetRT()->RC_EndFrame();
	}
}

bool Game::Update()
{
	static float t = 0;
	// t += m_Timer.DeltaTime();
	t += 0.001f;
	
	for(auto& body : solarSystem)
		body->UpdateVelocity(solarSystem, GravTimestep);

	for (auto& body : solarSystem) 
	{
		body->UpdatePosition(GravTimestep);
		m_pRenderEngine->GetRT()->RC_UpdateActorPosition(body->GetName(), body->GetPosition());
	}
		
	m_pRenderEngine->GetRT()->RC_OscillateCamera(sin(t));
	return true;
}

void Game::GenerateSolarSystem()
{
	CelestialBody* Sun = new CelestialBody("Sun");
	Sun->SetBodyParameters(1000, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0), false);
	m_pRenderEngine->GetRT()->RC_LoadActor("Sun", "penguin.mesh", Ogre::Vector3(0, 0, 0));
	//m_pRenderEngine->CreateSceneObject("Sun", "penguin.mesh");
	solarSystem.push_back(Sun);

	CelestialBody* Planet = new CelestialBody("Earth");
	Planet->SetBodyParameters(1, Ogre::Vector3(0, 0, 50), Ogre::Vector3(11.55f, 0, 0), true);
	m_pRenderEngine->GetRT()->RC_LoadActor("Earth", "robot.mesh", Ogre::Vector3(0, 0, 50));
	//m_pRenderEngine->CreateSceneObject("Earth", "robot.mesh");
	solarSystem.push_back(Planet);
}




