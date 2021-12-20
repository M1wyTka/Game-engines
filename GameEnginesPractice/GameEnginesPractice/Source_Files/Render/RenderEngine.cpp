#include "Render/RenderEngine.h"

RenderEngine::RenderEngine(ResourceManager* pResourceManager) :
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
	m_pResourceManager(pResourceManager)
{
	m_pRT = std::unique_ptr<RenderThread>(new RenderThread(this));
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_SDL_Window = SDL_CreateWindow("SDL Ogre Engine ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

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
	SDL_GL_MakeCurrent(m_SDL_Window, m_GL_Context);

	if (m_pRenderWindow->isVisible())
		m_bQuit |= !m_pRoot->renderOneFrame();
	
	SDL_GL_SwapWindow(m_SDL_Window);

	m_pRenderWindow->windowMovedOrResized();
}

void RenderEngine::RT_SetCurrentMouseState(bool isPressed, Ogre::Vector2 mousePos) 
{
	//m_bIsMousePressed = isPressed;
	//m_vecMousePos = mousePos;
}

void RenderEngine::RT_ProcessSDLEvent(SDL_Event event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			m_vecMousePos = Ogre::Vector2(x, y);
			RT_RaycastToMouse();
		}
	if (event.window.event == SDL_WINDOWEVENT_CLOSE)
		m_bQuit = true;
}

Ogre::SceneNode* RenderEngine::RT_RaycastToMouse()
{

	Ogre::Ray ray = m_pCamera->getCameraToViewportRay(float(m_vecMousePos.x) /m_pRenderWindow->getWidth(), float(m_vecMousePos.y) / m_pRenderWindow->getHeight());
	Ogre::RaySceneQuery* query = m_pSceneManager->createRayQuery(ray);
	query->setSortByDistance(true);

	Ogre::RaySceneQueryResult& result = query->execute();
	if (!result.empty()) 
	{
		m_bSelectionChanged = m_pCurSelection != result[0].movable->getParentSceneNode();
		m_pCurSelection = result[0].movable->getParentSceneNode();
	}
	
	m_pSceneManager->destroyQuery(query);
	return m_pCurSelection;
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
		compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue::ColourValue(0.85, 0.19, 0.06, 1));
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
	m_GL_Context = SDL_GL_CreateContext(m_SDL_Window);
	SDL_GL_MakeCurrent(m_SDL_Window, m_GL_Context);
	SDL_GL_SetSwapInterval(0.5);
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