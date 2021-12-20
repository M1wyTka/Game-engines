#include "Game.h"


Game::Game() :
	m_pRenderEngine(nullptr)
{

	const char* glsl_version = "#version 130";
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Create ecsworld
	m_pECSworld			= std::unique_ptr<flecs::world>(new flecs::world());

	// Systems
	m_pFileSystem		= std::unique_ptr<FileSystem>(new FileSystem());
	m_pInputHandler		= std::unique_ptr<InputHandler>(new InputHandler(m_pFileSystem->GetMediaRoot()));
	m_pResourceManager	= std::unique_ptr<ResourceManager>(new ResourceManager(m_pFileSystem.get()));

	m_pRenderEngine		= std::unique_ptr<RenderEngine>(new RenderEngine(m_pResourceManager.get()));
	m_pEditorWindow		= std::unique_ptr<EditorWindow>(new EditorWindow(m_pRenderEngine.get(), m_pFileSystem->GetScriptsRoot()));

	m_pScriptSystem		= std::unique_ptr<ScriptManager>(new ScriptManager(m_pInputHandler.get(), m_pFileSystem->GetScriptsRoot()));
	m_pEntityManager	= std::unique_ptr<EntityManager>(new EntityManager(m_pRenderEngine.get(), m_pScriptSystem.get(), m_pECSworld.get()));
	m_pProjectLoader	= std::unique_ptr<ProjectLoader>(new ProjectLoader(m_pRenderEngine.get(), m_pEntityManager.get()));
	
	//m_pEntityManager = std::unique_ptr<EntityManager>(new EntityManager(m_pRenderEngine.get(), m_pScriptSystem.get(), m_pECSworld.get()));
	
	// Handlers
	auto inputHandler = m_pECSworld->entity("inputHandler")
		.set(InputHandlerPtr{ m_pInputHandler.get() });
	auto renderEngine = m_pECSworld->entity("renderEngine")
		.set(RenderEnginePtr{ m_pRenderEngine.get() });

	LoadMeshSystems(*m_pECSworld.get());
	LoadControlSystems(*m_pECSworld.get());
	LoadPhysSystems(*m_pECSworld.get());
	LoadKinematicsSystems(*m_pECSworld.get());
	LoadScriptSystems(*m_pECSworld.get());
	//GUISceneHierarchy(*m_pECSworld.get());
	
	m_Timer.Start();
}

Game::~Game()
{
	// I think unique_pts should do the trick

}

void Game::Run()
{
	m_Timer.Reset();

	m_pRenderEngine->GetRT()->RC_EndFrame();
	LoadGameWorld();

	while (!m_pRenderEngine->GetQuit())
	{
		//	/*
		//	Possible additions:
		//		I have to add 2 different states for game engine: editor state and game state
		//		Key binding system
		//		flecs in parallel (OnUpdate part) (if it's even possible)
		//		flecs prefabs
		//		flecs scenes
		//		cullings
		//		add collision
		//		imgui support
		//		serialization
		//		editor of all objects, present in window
		//	*/
		if (!UpdateAllSystems())
			return;
	}
}

bool Game::UpdateAllSystems() 
{
	m_pRenderEngine->GetRT()->RC_BeginFrame();
	
	m_pEditorWindow->Update();

	m_pRenderEngine->GetRT()->RC_LambdaAction([&] {
		//m_pRenderEngine->RT_SetCurrentMouseState(m_pInputHandler->IsMousePressed(), m_pInputHandler->GetMousePosition());
		m_pEditorWindow->SetSelection(m_pRenderEngine->GetSelection());
		
	});

	if (m_pInputHandler)
		m_pInputHandler->Update();

	if (m_pInputHandler->IsQuit() || m_pEditorWindow->IsQuit() || !Update())
	{
		m_pRenderEngine->GetRT()->RC_LambdaAction([=] { 
			m_pRenderEngine->RT_SDLClenup();
		});

		m_pProjectLoader->SaveProject(m_pFileSystem->GetProjectSaveFile(), m_pRenderEngine->GetRenderedObjects());
		
		return false;
	}

	
	m_Timer.Tick();
	m_pRenderEngine->GetRT()->RC_EndFrame();

	return true;
}

bool Game::LoadGameWorld() 
{
	while (!m_pRenderEngine->IsInitialized())
	{}
	GenerateSolarSystem();
	return true;
}

bool Game::Update()
{
	if(!m_pEditorWindow->IsFrozen())
		m_pECSworld->progress();
	return true;
}

void Game::GenerateSolarSystem()
{
	m_pProjectLoader->GetLevelEntities("");
	// V = sqrt(G*M/r)
	auto player = m_pECSworld->entity()
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 0.f) , Ogre::Vector3(5, 5, 5) })
		.set(SceneObjectPattern{ Ogre::String("Player"), Ogre::String("ogrehead.mesh"), true });


	auto xAxis = m_pECSworld->entity()
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(25.f, 0.f, 0.f) , Ogre::Vector3(5.f, 0.3f, 0.3f) })
		.set(SceneObjectPattern{ Ogre::String("xAxis"), Ogre::String("Cube.mesh"), true });

	auto yAxis = m_pECSworld->entity()
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 25.f, 0.f) , Ogre::Vector3(0.3f, 5.f, 0.3f) })
		.set(SceneObjectPattern{ Ogre::String("yAxis"), Ogre::String("Cube.mesh"), true });

	auto zAxis = m_pECSworld->entity()
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 25.f) , Ogre::Vector3(0.3f, 0.3f, 5.f) })
		.set(SceneObjectPattern{ Ogre::String("zAxis"), Ogre::String("Cube.mesh"), true });

	auto test = m_pECSworld->entity()
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(60.f, 0.f, 0.f) , Ogre::Vector3(1.f, 1.f, 1.f) })
		.set(SceneObjectPattern{ Ogre::String("test"), Ogre::String("Cube.mesh"), true });

	auto sun = m_pECSworld->entity()
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 0.f) , Ogre::Vector3(1.f, 1.f, 1.f) })
		.set(SceneObjectPattern{ Ogre::String("sun"), Ogre::String("penguin.mesh"), true })

		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Mass{ 50000 })
		.set(DeltaKinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 0.f) , Ogre::Vector3(1.f, 1.f, 1.f) });

	auto earth = m_pECSworld->entity()
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 50.f) , Ogre::Vector3(1.f, 1.f, 1.f) })
		.set(SceneObjectPattern{ Ogre::String("earth"), Ogre::String("Sphere.mesh"), true })

		.set(Velocity{ 51.67f, 0.f, 0.f })
		.set(Mass{ 1 })
		.set(DeltaKinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 0.f) , Ogre::Vector3(1.f, 1.f, 1.f) });
		//.set(SphereCollider{ 5.f });

	auto mars = m_pECSworld->entity()
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 100.f) , Ogre::Vector3(1.f, 1.f, 1.f) })
		.set(SceneObjectPattern{ Ogre::String("mars"), Ogre::String("Sphere.mesh"), true })

		.set(Velocity{ 8.17f, 0.f, 0.f })
		.set(Mass{ 1 })
		.set(DeltaKinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 0.f) , Ogre::Vector3(1.f, 1.f, 1.f) });
		//.set(SphereCollider{ 5.f });

	auto cam = m_pECSworld->entity()
		.set(MainCameraPtr{ m_pRenderEngine->GetMainCamera() })
		.set(Kinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , m_pRenderEngine->GetMainCamera()->getPosition() , Ogre::Vector3(1.f, 1.f, 1.f) })
		.set(Controllable{ 50.f })
		.set(DeltaKinematics{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(0.f, 0.f, 0.f) , Ogre::Vector3(1.f, 1.f, 1.f) });
}




