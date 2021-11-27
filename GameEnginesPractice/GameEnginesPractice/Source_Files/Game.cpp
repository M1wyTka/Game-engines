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
	LoadKinematicsSystems(*m_pECSworld.get());
	
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
			m_pInputHandler->SetWinHandle(m_pRenderEngine->GetWinHandle());
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
		.set(Position{ Ogre::Vector3(0.f, 0.f, 0.f) })
		.set(Rotation{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) })
		.set(Scale{ Ogre::Vector3(5, 5, 5) })
		.set(MeshName{ Ogre::String("ogrehead.mesh") })

		.set(Controllable{ 5.0f })
		.set(DeltaPos{});
	

	auto xAxis = m_pECSworld->entity()
		.set(Position{ Ogre::Vector3(25.f, 0.f, 0.f) })
		.set(Rotation{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) })
		.set(Scale{ Ogre::Vector3(5.f, 0.3f, 0.3f) })
		.set(MeshName{ Ogre::String("Cube.mesh") });
		//.set(DeltaKinematics{});
	
	auto yAxis = m_pECSworld->entity()
		.set(Position{ Ogre::Vector3(0.f, 25.f, 0.f) })
		.set(Rotation{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) })
		.set(Scale{ Ogre::Vector3(0.3f, 5.f, 0.3f) })
		.set(MeshName{ Ogre::String("Cube.mesh") });
		//.set(DeltaKinematics{});

	auto zAxis = m_pECSworld->entity()
		.set(Position{ Ogre::Vector3(0.f, 0.f, 25.f) })
		.set(Rotation{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) })
		.set(Scale{ Ogre::Vector3(0.3f, 0.3f, 5.f) })
		.set(MeshName{ Ogre::String("Cube.mesh") });
		//.set(DeltaKinematics{});


	auto sun = m_pECSworld->entity()
		.set(Position{ Ogre::Vector3(0.f, 0.f, 0.f) })
		.set(Rotation{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) })
		.set(Scale{ Ogre::Vector3(1.f, 1.f, 1.f) })
		.set(MeshName{ Ogre::String("penguin.mesh") })

		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Mass{ 50000 })
		.set(DeltaPos{ Ogre::Vector3(0.f, 0.f, 0.f) });

	auto earth = m_pECSworld->entity()
		.set(Position{ Ogre::Vector3(0.f, 0.f, 50.f) })
		.set(Rotation{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) })
		.set(Scale{ Ogre::Vector3(1.f, 1.f, 1.f) })
		.set(MeshName{ Ogre::String("Sphere.mesh") })
		
		.set(Velocity{ 51.67f, 0.f, 0.f })
		.set(Mass{ 1 })
		.set(DeltaPos{ Ogre::Vector3(0.f, 0.f, 0.f) });

	auto mars = m_pECSworld->entity()
		.set(Position{ Ogre::Vector3(0.f, 0.f, 100.f) })
		.set(Rotation{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) })
		.set(Scale{ Ogre::Vector3(1.f, 1.f, 1.f) })
		.set(MeshName{ Ogre::String("Sphere.mesh") })

		.set(Velocity{ 8.17f, 0.f, 0.f })
		.set(Mass{ 1 })
		.set(DeltaPos{ Ogre::Vector3(0.f, 0.f, 0.f) });

	auto cam = m_pECSworld->entity()
		.set(MainCameraPtr{ m_pRenderEngine->GetMainCamera() })
		.set(Position{ m_pRenderEngine->GetMainCamera()->getPosition() } )
		.set(Controllable{ 5.0f })
		.set(DeltaPos{ Ogre::Vector3(0.f, 0.f, 0.f) });
}




