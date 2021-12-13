#include "Render/RenderEngine.h"

RenderEngine::RenderEngine(ResourceManager* pResourceManager, flecs::world* world) :
	m_pRoot(nullptr),
	m_pRenderWindow(nullptr),
	m_pSceneManager(nullptr),
	m_pD3D11Plugin(nullptr),
	m_pCamera(nullptr),
	m_pWorkspace(nullptr),
	m_pRT(nullptr),
	m_pSceneObjectProducer(nullptr),
	m_pCurSelection(nullptr),
	m_bIsInitialized(false),
	m_bQuit(false),
	m_bSelectionChanged(false),
	m_bIsFreeze(false),
	m_pResourceManager(pResourceManager),
	m_pECSWorld(world)
{
	m_pRT = std::unique_ptr<RenderThread>(new RenderThread(this));

	m_pRT->RC_LambdaAction([this] {
		RT_Init();
		RT_SetupDefaultCamera();
		RT_SetupDefaultCompositor();
		RT_LoadDefaultResources();
		RT_SetupDefaultLight();
		RT_LoadOgreHead();
	});
	
	m_pRT->Start();
}

RenderEngine::~RenderEngine()
{
	//SAFE_OGRE_DELETE(m_pRoot);
}

bool RenderEngine::SetOgreConfig()
{
#ifdef _DEBUG
	constexpr bool bAlwaysShowConfigWindow = true;
	if (bAlwaysShowConfigWindow || !m_pRoot->restoreConfig())
#else
	if (!m_pRoot->restoreConfig())
#endif
	{
		if (!m_pRoot->showConfigDialog())
		{
			return false;
		}
	}

	return true;
}

void RenderEngine::Update()
{
	Ogre::WindowEventUtilities::messagePump();

	if (m_pRenderWindow->isVisible())
		m_bQuit |= !m_pRoot->renderOneFrame();
	
	SDL_PumpEvents();
	{
		SDL_Event event;
		while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_TEXTINPUT) > 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
		}
	}

	if (m_bIsMousePressed)
		RaycastToMouse();

	StartGuiUpdate();
	
	DisplayMenuBar();
	DisplayAllScripts();
	DisplaySelectionParameters();
	DisplayFreezeBtn();

	EndGuiUpdate();
	
	m_pRenderWindow->windowMovedOrResized();
}

void RenderEngine::RT_SetCurrentMouseState(bool isPressed, Ogre::Vector2 mousePos) 
{
	m_bIsMousePressed = isPressed;
	m_vecMousePos = mousePos;
}

void RenderEngine::DisplayMenuBar() 
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "CTRL+S")) {}
			if (ImGui::MenuItem("Load", "")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void RenderEngine::DisplaySelectionParameters()
{
	if (m_pCurSelection)
	{
		ImGui::Begin("Parameters");
		ImGui::Text(m_pCurSelection->getName().c_str());
		
		static float posVec[3] = { 0.f, 0.f, 0.f};
		posVec[0] = float(m_pCurSelection->getPosition().x);
		posVec[1] = float(m_pCurSelection->getPosition().y);
		posVec[2] = float(m_pCurSelection->getPosition().z);
		ImGui::InputFloat3("Position", posVec);

		static int rotVec[3] = { 0, 0, 0 };
		rotVec[0] = int(m_pCurSelection->getOrientation().getPitch().valueDegrees());
		rotVec[1] = int(m_pCurSelection->getOrientation().getYaw().valueDegrees());
		rotVec[2] = int(m_pCurSelection->getOrientation().getRoll().valueDegrees());
		ImGui::InputInt3("Rotation", rotVec);
		ImGui::End();

		m_pCurSelection->setPosition(posVec[0], posVec[1], posVec[2]);

		Ogre::Quaternion q = m_pCurSelection->getOrientation();
		int clampedX = std::clamp(rotVec[0], -180, 180);
		if (clampedX != int(m_pCurSelection->getOrientation().getPitch().valueDegrees()))
		{
			Ogre::Radian radX = Ogre::Radian(Ogre::Degree(clampedX));
			Ogre::Radian offsetX = radX - m_pCurSelection->getOrientation().getPitch();
			q.FromAngleAxis(offsetX, Ogre::Vector3::UNIT_X);
			q = q* m_pCurSelection->getOrientation();
			m_pCurSelection->setOrientation(q);
			return;
		}

		int clampedY = std::clamp(rotVec[1], -180, 180);
		if (clampedY != int(m_pCurSelection->getOrientation().getYaw().valueDegrees()))
		{
			Ogre::Radian radY = Ogre::Radian(Ogre::Degree(clampedY));
			Ogre::Radian offsetY = radY - m_pCurSelection->getOrientation().getYaw();
			q.FromAngleAxis(offsetY, Ogre::Vector3::UNIT_Y);
			q = q * m_pCurSelection->getOrientation();
			m_pCurSelection->setOrientation(q);
			return;
		}

		int clampedZ = std::clamp(rotVec[2], -180, 180);
		if (clampedZ != int(m_pCurSelection->getOrientation().getRoll().valueDegrees()))
		{
			Ogre::Radian radZ = Ogre::Radian(Ogre::Degree(clampedZ));
			Ogre::Radian offsetZ = radZ - m_pCurSelection->getOrientation().getRoll();
			q.FromAngleAxis(offsetZ, Ogre::Vector3::UNIT_Z);
			q = q * m_pCurSelection->getOrientation();
			m_pCurSelection->setOrientation(q);
			return;
		}
	}
}

void RenderEngine::DisplayAllScripts()
{
	ImGui::Begin("Scripts");
	std::string path = "D:\\MIPT\\Game-engines\\GameEnginesPractice\\GameEnginesPractice\\Scripts";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".lua")
		{
			std::string btnName = entry.path().filename().string();
			if (ImGui::Button(btnName.c_str()))
			{
				std::string command = "start notepad++ " + entry.path().string();
				std::system(command.c_str());
			}
		}
	}
	ImGui::End();
}

void RenderEngine::DisplayFreezeBtn() 
{
	ImGui::Begin("lol");
	ImGui::Checkbox("F", &m_bIsFreeze);
	ImGui::End();
}

void RenderEngine::RaycastToMouse()
{
	Ogre::Ray ray = m_pCamera->getCameraToViewportRay(float(m_vecMousePos.x) /m_pRenderWindow->getWidth(), float(m_vecMousePos.y) / m_pRenderWindow->getHeight());
	Ogre::RaySceneQuery* query = m_pSceneManager->createRayQuery(ray);
	query->setSortByDistance(true);

	bool mMovableFound = false;
	Ogre::RaySceneQueryResult& result = query->execute();
	if (!result.empty()) 
	{
		m_bSelectionChanged = m_pCurSelection != result[0].movable->getParentSceneNode();
		m_pCurSelection = result[0].movable->getParentSceneNode();
	}
	
	m_pSceneManager->destroyQuery(query);
}

void RenderEngine::StartGuiUpdate() 
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_SDL_Window);
	ImGui::NewFrame();
}

void RenderEngine::EndGuiUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_SDL_Window);
	m_pRenderWindow->windowMovedOrResized();
}

void RenderEngine::RT_Init()
{
	m_pRoot = std::unique_ptr<Ogre::Root>(new Ogre::Root());
	m_pD3D11Plugin = std::unique_ptr<Ogre::D3D11Plugin>(new Ogre::D3D11Plugin());
	m_pGL3PlusPlugin = std::unique_ptr<Ogre::GL3PlusPlugin>(new Ogre::GL3PlusPlugin());

	m_pRoot->installPlugin(m_pD3D11Plugin.get());
	m_pRoot->installPlugin(m_pGL3PlusPlugin.get());

	if (!SetOgreConfig())
	{
		m_bQuit = true;
		return;
	}

	m_pRoot->initialise(false);

	RT_InitSDL();

	// Creating window
	Ogre::uint32 width = 1280;
	Ogre::uint32 height = 720;
	Ogre::String sTitleName = "Game Engine";

	Ogre::NameValuePairList params;
	params.insert(std::make_pair("title", sTitleName));
	params.insert(std::make_pair("gamma", "true"));
	params.insert(std::make_pair("FSAA", "false"));
	params.insert(std::make_pair("vsync", "false"));

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(m_SDL_Window, &info);

	params["externalWindowHandle"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(info.info.win.window));
	params["externalGLContent"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(info.info.win.hdc));
	params["externalGLControl"] = Ogre::String("True");
	
	m_pRenderWindow = std::unique_ptr<Ogre::Window>(Ogre::Root::getSingleton().createRenderWindow(sTitleName, width, height, false, &params));


	// Scene manager
	m_pSceneManager = std::unique_ptr<Ogre::SceneManager>(m_pRoot->createSceneManager(Ogre::SceneType::ST_GENERIC, 2));
	m_pSceneObjectProducer = std::unique_ptr<SceneObjectProducer>(new SceneObjectProducer(m_pSceneManager.get()));

}

void RenderEngine::RT_SetupDefaultCamera()
{
	m_pCamera = std::unique_ptr<Ogre::Camera>(m_pSceneManager->createCamera("Main Camera"));

	m_pCamera->setPosition(Ogre::Vector3(150, 150, 150));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(0.2f);
	m_pCamera->setFarClipDistance(1000.0f);
	m_pCamera->setAutoAspectRatio(true);
}

void RenderEngine::RT_SetupDefaultCompositor()
{
	Ogre::CompositorManager2* compositorManager = m_pRoot->getCompositorManager2();

	const Ogre::String workspaceName("WorkSpace");

	if (!compositorManager->hasWorkspaceDefinition(workspaceName))
	{
		compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue::Blue);
	}
	
	m_pWorkspace = std::unique_ptr<Ogre::CompositorWorkspace>
					(compositorManager->addWorkspace(m_pSceneManager.get(), m_pRenderWindow->getTexture(), m_pCamera.get(), workspaceName, true));
}

void RenderEngine::RT_LoadDefaultResources()
{
	m_pResourceManager->LoadDefaultResources();
}

void RenderEngine::RT_LoadOgreHead()
{
	m_bIsInitialized = true;
}

void RenderEngine::RT_SetupDefaultLight()
{
	// Lightning
	Ogre::Light* light = m_pSceneManager->createLight();
	Ogre::SceneNode* lightNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());
}

SceneObject* RenderEngine::RT_CreateSceneObject(Ogre::String actorName, Ogre::String meshName) 
{
	return m_pSceneObjectProducer->Produce(actorName, meshName);
}

void RenderEngine::RT_InitSDL() 
{
	SDL_Init(SDL_INIT_VIDEO);

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_SDL_Window = SDL_CreateWindow("SDL Ogre Engine ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

	m_GL_Context = SDL_GL_CreateContext(m_SDL_Window);
	SDL_GL_MakeCurrent(m_SDL_Window, m_GL_Context);
	SDL_GL_SetSwapInterval(0.5);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantCaptureKeyboard = true;
	Ogre::LogManager::getSingleton().logMessage(std::to_string(io.WantCaptureKeyboard));

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_SDL_Window, m_GL_Context);
	bool f = ImGui_ImplOpenGL3_Init(glsl_version);
}

void RenderEngine::RT_SDLClenup()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(m_GL_Context);
	SDL_DestroyWindow(m_SDL_Window);
	SDL_Quit();
}