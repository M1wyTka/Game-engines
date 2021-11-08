#include "Game.h"


Game::Game() :
	m_pRenderEngine(nullptr)
{
	// Create ecsworld
	m_pECSworld = std::unique_ptr<flecs::world>(new flecs::world());

	// Systems
	m_pFileSystem = std::unique_ptr<FileSystem>(new FileSystem());
	m_pInputHandler = std::unique_ptr<InputHandler>(new InputHandler(m_pFileSystem->GetMediaRoot()));
	m_pResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager(m_pFileSystem->GetMediaRoot()));
	m_pRenderEngine = std::unique_ptr<RenderEngine>(new RenderEngine(m_pResourceManager.get()));
	//m_pScriptSystem = std::unique_ptr<ScriptSystem>(new ScriptSystem(m_pInputHandler.get(), m_pFileSystem->GetScriptsRoot()));
	//m_pEntityManager = std::unique_ptr<EntityManager>(new EntityManager(m_pRenderEngine.get(), m_pScriptSystem.get(), m_pECSworld.get()));

	// Create script system


	// Handlers
	auto inputHandler = m_pECSworld->entity("inputHandler")
		.set(InputHandlerPtr{ m_pInputHandler.get() });
	auto renderEngine = m_pECSworld->entity("renderEngine")
		.set(RenderEnginePtr{ m_pRenderEngine.get() });

	// Test entities


	LoadMeshSystems(*m_pECSworld.get());
	LoadControlSystems(*m_pECSworld.get());
	LoadPhysSystems(*m_pECSworld.get());
	
	m_Timer.Start();
}

Game::~Game()
{
	// I think unique_pts should do the trick

}

void Game::Run()
{
	m_Timer.Reset();
	bool isDone = false;
	
	while (true)
	{
		m_pRenderEngine->GetRT()->RC_BeginFrame();

		if (!isDone && m_pRenderEngine->IsInitialized()) 
		{
			GenerateSolarSystem();
			isDone = true;
		} 

		if (m_pInputHandler)
			m_pInputHandler->Update();

		m_Timer.Tick();
				
		if(!Update())
			break;

		m_pRenderEngine->GetRT()->RC_EndFrame();
	}
}

bool Game::Update()
{
	m_pECSworld->progress();
	return true;
}

void Game::GenerateSolarSystem()
{
	// V = sqrt(G*M/r)
	auto player = m_pECSworld->entity()
		.set(Position{ 0.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(MeshName{ Ogre::String("ogrehead.mesh") })
		.set(Controllable{ 5.0f })
		.set(Scale{ 5, 5, 5 });
	

	auto xAxis = m_pECSworld->entity()
		.set(Position{ 25.f, 0.f, 0.f })
		.set(MeshName{ Ogre::String("Cube.mesh") })
		.set(Scale{ 5.f, 0.3f, 0.3f });

	auto yAxis = m_pECSworld->entity()
		.set(Position{ 0.f, 25.f, 0.f })
		.set(MeshName{ Ogre::String("Cube.mesh") })
		.set(Scale{ 0.3f, 5.f, 0.3f });

	auto zAxis = m_pECSworld->entity()
		.set(Position{ 0.f, 0.f, 25.f })
		.set(MeshName{ Ogre::String("Cube.mesh") })
		.set(Scale{ 0.3f, 0.3f, 5.f });


	auto sun = m_pECSworld->entity()
		.set(Position{ 0.f, 0.f, 0.f })
		.set(MeshName{ Ogre::String("penguin.mesh") })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Mass{ 50000 });

	auto earth = m_pECSworld->entity()
		.set(Position{ 0.f, 0.f, 50.f })
		.set(MeshName{ Ogre::String("Sphere.mesh") })
		.set(Velocity{ 81.91f, 0.f, 0.f })
		.set(Mass{ 1 });

	auto mars = m_pECSworld->entity()
		.set(Position{ 0.f, 0.f, 100.f })
		.set(MeshName{ Ogre::String("Sphere.mesh") })
		.set(Velocity{ 8.17f, 0.f, 0.f })
		.set(Mass{ 1 });
}




