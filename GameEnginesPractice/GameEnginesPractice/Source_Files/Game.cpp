#include "Game.h"


Game::Game() :
	m_pRenderEngine(nullptr)
{
	m_pFileSystem = new FileSystem();
	m_pInputHandler = new InputHandler(m_pFileSystem->GetMediaRoot());
	m_pResourceManager = new ResourceManager(m_pFileSystem->GetMediaRoot());
	m_pRenderEngine = new RenderEngine(m_pResourceManager);


	auto inputHandler = world.entity("inputHandler")
		.set(InputHandlerPtr{ m_pInputHandler });
	auto renderEngine = world.entity("renderEngine")
		.set(RenderEnginePtr{ m_pRenderEngine });

	LoadMeshSystems(world);
	LoadControlSystems(world);
	LoadPhysSystems(world);
	
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

		
		if(i==0) GenerateSolarSystem();

		if (m_pInputHandler)
			m_pInputHandler->Update();

		m_Timer.Tick();
		
		
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

	if (m_pInputHandler->GetInputState().test(eIC_GoUp))
		auto ogreHeadControl = world.entity()
		.set(Position{ 0.f, 10.f, 0.f })
		.set(Velocity{ 0.f, 10.f, 0.f })
		.set(MeshName{ Ogre::String("cube.mesh") })
		.set(Controllable{ 5.0f });

	world.progress();

	/*for(auto& body : solarSystem)
		body->UpdateVelocity(solarSystem, GravTimestep);

	for (auto& body : solarSystem) 
	{
		body->UpdatePosition(GravTimestep);
		m_pRenderEngine->GetRT()->RC_UpdateActorPosition(&body->GetActor(), body->GetPosition());
	}
	
	if(m_pInputHandler->GetInputState().test(eIC_GoLeft))
		m_pRenderEngine->GetRT()->RC_MoveCamera(Ogre::Vector3(-25.0f * GravTimestep, 0, 0));

	if (m_pInputHandler->GetInputState().test(eIC_GoRight))
		m_pRenderEngine->GetRT()->RC_MoveCamera(Ogre::Vector3(25.0f * GravTimestep, 0, 0));

	if(m_pInputHandler->GetInputState().test(eIC_GoUp))
		m_pRenderEngine->GetRT()->RC_MoveCamera(Ogre::Vector3(0, 25.0f * GravTimestep, 0));

	if (m_pInputHandler->GetInputState().test(eIC_GoDown))
		m_pRenderEngine->GetRT()->RC_MoveCamera(Ogre::Vector3(0, -25.0f * GravTimestep, 0));*/
	
	/*if (m_pInputHandler->GetInputState().test(eIC_Shoot))
	{
		SceneObject* bullet = m_pRenderEngine->CreateSceneObject("bullet", "Sphere.mesh");
		Bullet* bul = new Bullet(Ogre::Vector3(0, 0, 0), Ogre::Vector3(5, 0, 0), true);
		bul->SetSceneObject(bullet);
		bullets.push_back(bul);
	}
	
	for (auto& bullet : bullets)
	{
		bullet->Update(GravTimestep);
		m_pRenderEngine->GetRT()->RC_UpdateActorPosition(&bullet->GetSceneObject(), bullet->GetPosition());
	}*/

	/*if (m_pInputHandler->GetInputState().test(eIC_Shoot))
		m_pRenderEngine->CreateSceneObject()*/
	
	return true;
}

void Game::GenerateSolarSystem()
{
	// V = sqrt(G*M/r)
	/*std::unique_ptr<CelestialBody> Sun = std::make_unique<CelestialBody>();
	Sun->SetBodyParameters(50000, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0), true);
	Sun->SetSceneObject(m_pRenderEngine->CreateSceneObject("Sun", "Sphere.mesh"));
	solarSystem.push_back(std::move(Sun));

	std::unique_ptr<CelestialBody> Earth = std::make_unique<CelestialBody>();
	Earth->SetBodyParameters(1, Ogre::Vector3(0, 0, 50), Ogre::Vector3(81.91f, 0, 0), true);
	Earth->SetSceneObject(m_pRenderEngine->CreateSceneObject("Sun", "Sphere.mesh"));
	solarSystem.push_back(std::move(Earth));

	std::unique_ptr<CelestialBody> Mars = std::make_unique<CelestialBody>();
	Mars->SetBodyParameters(1, Ogre::Vector3(0, 0, 100), Ogre::Vector3(8.17f, 0, 0), true);
	Mars->SetSceneObject(m_pRenderEngine->CreateSceneObject("Mars", "Sphere.mesh"));
	solarSystem.push_back(std::move(Mars));*/

	auto ogreHeadControl = world.entity()
		.set(Position{ 0.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(MeshName{ Ogre::String("ogrehead.mesh") })
		.set(Controllable { 5.0f });

	auto xAxis = world.entity()
		.set(Position{ 50.f, 0.f, 0.f })
		.set(MeshName{ Ogre::String("Cube.mesh") });

	auto Cube = world.entity()
		.set(Position{ -50.f, 0.f, 0.f })
		.set(MeshName{ Ogre::String("Sphere.mesh") });

	auto Barrel = world.entity()
		.set(Position{ 0.f, 0.f, 0.f })
		.set(MeshName{ Ogre::String("penguin.mesh") });
}




