#include "Game.h"

Game::Game() :
	m_pRenderEngine(nullptr)
{
	m_pFileSystem = new FileSystem();
	m_pInputHandler = new InputHandler(m_pFileSystem->GetMediaRoot());
	m_pResourceManager = new ResourceManager(m_pFileSystem->GetMediaRoot());
	m_pRenderEngine = new RenderEngine(m_pResourceManager);

	m_Timer.Start();
}

Game::~Game()
{
}

void Game::Run()
{
	m_Timer.Reset();
	int i = 0;
	
	while (true)
	{
		m_pRenderEngine->GetRT()->RC_BeginFrame();

		if(i == 300)
			GenerateSolarSystem();

		if (m_pInputHandler)
			m_pInputHandler->Update();

		m_Timer.Tick();
		
		if (i >= 300)
			if(!Update())
				break;

		i++;
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
		m_pRenderEngine->GetRT()->RC_UpdateActorPosition(&body->GetActor(), body->GetPosition());
	}
	
	if(m_pInputHandler->GetInputState().test(eIC_GoLeft))
		m_pRenderEngine->GetRT()->RC_OscillateCamera(5.0f * GravTimestep);

	if (m_pInputHandler->GetInputState().test(eIC_GoRight))
		m_pRenderEngine->GetRT()->RC_OscillateCamera(-5.0f * GravTimestep);

	/*if (m_pInputHandler->GetInputState().test(eIC_Shoot))
		m_pRenderEngine->CreateSceneObject()*/
	
	return true;
}

void Game::GenerateSolarSystem()
{
	// V = sqrt(G*M/r)
	std::unique_ptr<CelestialBody> Sun = std::make_unique<CelestialBody>("Sun");
	Sun->SetBodyParameters(50000, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0), true);
	Sun->SetSceneNode(m_pRenderEngine->CreateSceneObject("Sun", "Sphere.mesh"));
	solarSystem.push_back(std::move(Sun));

	std::unique_ptr<CelestialBody> Earth = std::make_unique<CelestialBody>("Earth");
	Earth->SetBodyParameters(1, Ogre::Vector3(0, 0, 50), Ogre::Vector3(81.91f, 0, 0), true);
	Earth->SetSceneNode(m_pRenderEngine->CreateSceneObject("Sun", "Sphere.mesh"));
	solarSystem.push_back(std::move(Earth));

	std::unique_ptr<CelestialBody> Mars = std::make_unique<CelestialBody>("Earth");
	Mars->SetBodyParameters(1, Ogre::Vector3(0, 0, 100), Ogre::Vector3(8.17f, 0, 0), true);
	Mars->SetSceneNode(m_pRenderEngine->CreateSceneObject("Mars", "Sphere.mesh"));
	solarSystem.push_back(std::move(Mars));
}




